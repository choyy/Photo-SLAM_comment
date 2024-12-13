#include <chrono>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>

#include <torch/torch.h>
#include "ORB-SLAM3/include/System.h"
#include "include/gaussian_mapper.h"
#include "viewer/imgui_viewer.h"


int main(int argc, char** argv)
{
    std::string voc_file = "./ORB-SLAM3/Vocabulary/ORBvoc.txt";
    std::string cam_params = "./cfg/ORB_SLAM3/Monocular/TUM/usb_cam.yaml";
    std::string gaussian_mapper_params = "./cfg/gaussian_mapper/Monocular/TUM/tum_mono.yaml";

    std::string output_directory = std::string("./results/tum_mono/usb_cam/");
    if (output_directory.back() != '/')
        output_directory += "/";
    std::filesystem::path output_dir(output_directory);
    bool use_viewer = true;
    // Device
    torch::DeviceType device_type;
    if (torch::cuda::is_available()) {
        std::cout << "CUDA available! Training on GPU." << std::endl;
        device_type = torch::kCUDA;
    }
    else {
        std::cout << "Training on CPU." << std::endl;
        device_type = torch::kCPU;
    }
    // 声明SLAM系统
    std::shared_ptr<ORB_SLAM3::System> pSLAM =
        std::make_shared<ORB_SLAM3::System>(
            voc_file, cam_params, ORB_SLAM3::System::MONOCULAR);
    float imageScale = pSLAM->GetImageScale();

    // Create GaussianMapper
    std::filesystem::path gaussian_cfg_path(gaussian_mapper_params);
    std::shared_ptr<GaussianMapper> pGausMapper =
        std::make_shared<GaussianMapper>(
            pSLAM, gaussian_cfg_path, output_dir, 0, device_type);
    std::thread training_thd(&GaussianMapper::run, pGausMapper.get());

    // Create Gaussian Viewer
    std::thread viewer_thd;
    std::shared_ptr<ImGuiViewer> pViewer;
    if (use_viewer) {
        pViewer = std::make_shared<ImGuiViewer>(pSLAM, pGausMapper);
        viewer_thd = std::thread(&ImGuiViewer::run, pViewer.get());
    }


    cv::VideoCapture cap("/dev/video0");
    if (!cap.isOpened())
    {
        printf("Can not open a camera\n");
        return -1;
    }

    cv::Mat frame;
    cap >> frame;
    printf("image size: %d x %d\n", frame.cols, frame.rows);

    auto t_last = std::chrono::steady_clock::now();
    auto start = std::chrono::system_clock::now();
    while (true)
    {
        cap >> frame;

        if (frame.empty()) {
            break;
        }
        auto now = chrono::system_clock::now();
        auto timestamp = chrono::duration_cast<chrono::milliseconds>(now - start);
        double tframe = double(timestamp.count())/1000.;

        if (pSLAM->isShutDown()) break;
        if (imageScale != 1.f)
        {
            int width = frame.cols * imageScale;
            int height = frame.rows * imageScale;
            cv::resize(frame, frame, cv::Size(width, height));
        }
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

        // imshow("frame", frame);
        // Pass the image to the SLAM system
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB); // 3d高斯重建的颜色格式似乎有问题，必须把正常的图片格式转换一下
        pSLAM->TrackMonocular(frame, tframe, std::vector<ORB_SLAM3::IMU::Point>(), std::to_string(tframe));

        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

        double ttrack = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();



        // auto t_current = std::chrono::steady_clock::now();
        // printf("fps: %.2f\n", 1000.0 / std::chrono::duration_cast<std::chrono::milliseconds>(t_current - t_last).count());
        // t_last = t_current;

    }
    // Stop all threads
    pSLAM->Shutdown();
    training_thd.join();
    if (use_viewer)
        viewer_thd.join();
    // Save camera trajectory
    pSLAM->SaveTrajectoryTUM((output_dir / "CameraTrajectory_TUM.txt").string());
    pSLAM->SaveKeyFrameTrajectoryTUM((output_dir / "KeyFrameTrajectory_TUM.txt").string());
    pSLAM->SaveTrajectoryEuRoC((output_dir / "CameraTrajectory_EuRoC.txt").string());
    pSLAM->SaveKeyFrameTrajectoryEuRoC((output_dir / "KeyFrameTrajectory_EuRoC.txt").string());

    cv::waitKey(0);
    return 0;
}