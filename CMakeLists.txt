cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(VideoCheck)

set(CMAKE_PREFIX_PATH /home/andy/Qt/5.14.1/gcc_64/lib/cmake/Qt5)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 REQUIRED Widgets Core Gui)
qt5_wrap_cpp( MOC mainwindow.h)
qt5_wrap_ui( UIC mainwindow.ui)
qt5_add_resources(RCC resources.qrc)
qt5_generate_moc(main.cpp main.moc)

set_target_properties(Qt5::Core Qt5::Gui PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")

file(GLOB_RECURSE CURRENT_HEADERS *.h ${OpenCV_DIR}/include/*.h)
file(GLOB CURRENT_SOURCES *.cpp)

include_directories(BEFORE libosa/inc libosa/inc/csl )

set(CMAKE_PREFIX_PATH /home/andy/anaconda3/envs/py37_torch/lib/python3.7/site-packages/torch)
find_package(Torch REQUIRED NO_CMAKE_FIND_ROOT_PATH)
find_package (OpenCV REQUIRED NO_CMAKE_FIND_ROOT_PATH)

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

message(STATUS "Caffe2 library status:")
message(STATUS "    version: ${CAFFE2_VERSION}")
message(STATUS "    libraries: ${Caffe2_MAIN_LIBS}")
message(STATUS "    include path: ${CAFFE2_INCLUDE_DIRS}")


message(STATUS "Torch library status:")
message(STATUS "    version: ${TORCH_VERSION}")
message(STATUS "    libraries: ${TORCH_LIBRARIES}")
message(STATUS "    include path: ${TORCH_INCLUDE_DIRS}")

include_directories(  ${OpenCV_INCLUDE_DIRS}  ${TORCH_INCLUDE_DIRS} include)

add_executable(model_detect ${MOC} ${UIC} ${CURRENT_HEADERS} ${CURRENT_SOURCES} mainwindow.ui)

target_compile_features(model_detect PUBLIC cxx_range_for)

target_link_libraries(model_detect "${TORCH_LIBRARIES}"  Qt5::Widgets Qt5::Core Qt5::Gui)
target_link_libraries(model_detect "opencv_core" "opencv_highgui" "opencv_imgcodecs" "opencv_imgproc" "dl")
set_property(TARGET model_detect PROPERTY CXX_STANDARD 14)
