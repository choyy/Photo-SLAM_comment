#include <chrono>
#include <cstddef>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <set>
#include <string>

int main(int argc, char** argv)
{
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
    while (true)
    {
        cap >> frame;

        if (frame.empty()) {
            break;
        }
        imshow("frame", frame);
        auto t_current = std::chrono::steady_clock::now();
        printf("fps: %.2f\n", 1000.0 / std::chrono::duration_cast<std::chrono::milliseconds>(t_current - t_last).count());
        t_last = t_current;

        auto c { char(cv::waitKey(1)) };
        if (c == 'q') { // q键退出循环
            printf("quit\n");
            break;
        } else if (c == 's') { // s键保存图像
            static size_t count = 0;
            cv::imwrite("results/img/" + std::to_string(count++) + ".png", frame);
            printf("image saved\n");
        }
    }

    cv::waitKey(0);
    return 0;
}