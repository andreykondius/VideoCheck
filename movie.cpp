#include "movie.h"
#include "store.h"
#include <opencv2/opencv.hpp>
#include <torch/script.h>

Movie::Movie(const std::string &val) : fExit(false), fileName(val)
{
}

bool Movie::calc()
{
    torch::NoGradGuard no_grad;
    torch::jit::script::Module module;
    try
    {
        module = torch::jit::load( modelName );
    }
    catch(const c10::Error& e)
    {
        std::cerr << "error loading the model\n" << std::endl;
        return false;
    }

    cv::VideoCapture cap( fileName );

    if(!cap.isOpened())
        return false;

    store = std::make_shared<Store>();
    while(true)
    {
        if(fExit)
            break;
        cv::Mat srcFrame, dstFrame;
        if(!cap.read(srcFrame))
            break;
        const int width = srcFrame.cols;
        const int height = srcFrame.rows;
        cv::resize(srcFrame, dstFrame, cv::Size(widthHeight, widthHeight));

        if(dstFrame.channels() == 3)
        {
            cv::cvtColor(dstFrame, dstFrame, cv::COLOR_BGR2RGB);
        }
        else
            if(dstFrame.channels() == 4)
            {
                cv::cvtColor(dstFrame, dstFrame, cv::COLOR_BGRA2RGB);
            }
            else
                continue;

        dstFrame.convertTo(dstFrame, CV_32FC3, 1.0f / 255.0f);
        torch::Tensor tensor_img = torch::from_blob(dstFrame.data, {1, dstFrame.rows, dstFrame.cols, 3}, torch::kByte);
        tensor_img = tensor_img.permute({0, 3,1,2});
        tensor_img = tensor_img.toType(torch::kFloat);
        tensor_img = tensor_img.to(torch::kCPU);
        tensor_img = tensor_img.div(255);

        at::Tensor out_tensor = module.forward( {tensor_img} ).toTensor();

        out_tensor = out_tensor.mul(255).clamp(0, 255).to(torch::kU8);
        out_tensor = out_tensor.to(torch::kCPU);

        cv::Mat detectionMat(widthHeight, widthHeight, CV_32FC3);
        const int sz = sizeof(float)*out_tensor.numel();
        detectionMat.data = new uchar[sz];
        std::memcpy((void*)detectionMat.data, out_tensor.data_ptr(),sizeof(float)*out_tensor.numel());

        std::vector< std::vector<int> > bboxes;
        for(size_t i = 0; i < detectionMat.rows; i++)
        {
            int lables  = out_tensor.argmax().item().to<int>();
            float scores = detectionMat.at<float>(i, 2);
            if(lables == 1 && scores > 0.5)
            {
                int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * width);
                int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * height);
                int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * width);
                int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * height);
                std::cout << "lables = " << lables << ", scores = " << scores << ",  bounding boxes:  x1 = " << x1 << ", y1 = " << y1 << ", x2 = " << x2 << ", y2 = " << y2 << std::endl;
                store->push(x1,y1,x2,y2);
            }
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
        delete []detectionMat.data;
    }
    return true;
}

void Movie::onExit()
{
    fExit = true;
}

