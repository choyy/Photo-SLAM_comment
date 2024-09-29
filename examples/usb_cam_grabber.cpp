#include <chrono>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>


int main(int argc, char** argv)
{
    std::string output_directory = std::string("./results/img/");
    if (output_directory.back() != '/') { output_directory += "/"; }

    cv::VideoCapture cap("/dev/video0");
    if (!cap.isOpened()) {
        printf("Can not open a camera\n");
        return -1;
    }

    cv::Mat frame;
    cap >> frame;
    printf("image size: %d x %d\n", frame.cols, frame.rows);

    // auto start                 = std::chrono::system_clock::now();
    bool save_frame_constantly = false;
    while (true) {
        // std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
        cap >> frame;

        if (frame.empty()) { break; }
        // auto   now       = std::chrono::system_clock::now();
        // auto   timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
        // double tframe    = double(timestamp.count()) / 1000.;

        imshow("frame", frame);

        // std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
        // double ttrack = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
        // auto t_current = std::chrono::steady_clock::now();
        // printf("fps: %.2f\n", 1000.0 / std::chrono::duration_cast<std::chrono::milliseconds>(t_current - t_last).count());
        // t_last = t_current;

        auto c{char(cv::waitKey(1))};
        if (c == 'q') { // q键退出循环
            printf("quit\n");
            break;
        } else if (c == 's') { // s键保存图像
            static size_t count = 0;
            cv::imwrite(output_directory + std::to_string(++count) + ".png", frame);
            printf("image %zu saved\n", count);
        } else if (c == 'v') { // v键连续保存图像
            save_frame_constantly = true;
        }
        if (save_frame_constantly) {
            static size_t count_video = 0;
            cv::imwrite(output_directory + std::to_string(++count_video) + ".png", frame);
            printf("image %zu saved\n", count_video);
        }
    }

    cv::waitKey(0);
    return 0;
}