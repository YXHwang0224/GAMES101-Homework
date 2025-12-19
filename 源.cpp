#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <deque>

#pragma comment(lib,"opencv_world4100d.lib")

std::vector<cv::Point2f> control_points;

void mouse_handler(int event, int x, int y, int flags, void* userdata)
{
    if (event == cv::EVENT_LBUTTONDOWN && control_points.size() < 4)
    {
        std::cout << "Left button of the mouse is clicked - position (" << x << ", "
            << y << ")" << '\n';
        control_points.emplace_back(x, y);
    }
}

void naive_bezier(const std::vector<cv::Point2f>& points, cv::Mat& window)
{
    auto& p_0 = points[0];
    auto& p_1 = points[1];
    auto& p_2 = points[2];
    auto& p_3 = points[3];

    for (double t = 0.0; t <= 1.0; t += 0.001)
    {
        auto point = std::pow(1 - t, 3) * p_0 + 3 * t * std::pow(1 - t, 2) * p_1 +
            3 * std::pow(t, 2) * (1 - t) * p_2 + std::pow(t, 3) * p_3;

        window.at<cv::Vec3b>(point.y, point.x)[2] = 255;
    }
}

cv::Point2f recursive_bezier(const std::vector<cv::Point2f>& control_points, float t)
{
    // TODO: Implement de Casteljau's algorithm
    cv::Point2f front, back;
    std::deque<cv::Point2f> points;
    points.resize(control_points.size());
    std::copy(control_points.begin(), control_points.end(), points.begin());
    while (points.size() > 1){
        back = *(points.end()-1);
        points.pop_back();
        size_t i = points.size();
        while (i-- != 0) {
            front = back;
            back = *(points.end() - 1);
            points.pop_back();
            cv::Point2f new_point((1.0f - t) * back + t * front);
            points.push_front(new_point);
        }
    }
    return *points.begin();
}

void bezier(const std::vector<cv::Point2f>& control_points, cv::Mat& window)
{
    // TODO: Iterate through all t = 0 to t = 1 with small steps, and call de Casteljau's 
    // recursive Bezier algorithm.
    float step = 0.001f;
    float t = step;
    while (t < 1) {
        auto point = recursive_bezier(control_points, t);
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {//遍历9个像素
                if (point.y + j > 700 || point.y + j < 0 || point.x + i > 700 || point.x + i < 0)//不处理越界像素
                    continue;
                //计算ratio，距离最大为二点五倍根号二，所以将radio即颜色系数设置为距离与二点五倍根号二的比值
                float ratio = 1 - sqrt(2) * sqrt(pow(point.y - int(point.y + j) - 0.5, 2) + pow(point.x - int(point.x + i) - 0.5, 2)) / 2.5;
                //计算像素颜色
                window.at<cv::Vec3b>(point.y + j, point.x + i)[1] = std::fmax(window.at<cv::Vec3b>(point.y + j, point.x + i)[1], 255 * ratio);
            }
        }
        //window.at<cv::Vec3b>(point.y, point.x)[2] = 255;
        t += step;
    }
}

int main()
{
    cv::Mat window = cv::Mat(700, 700, CV_8UC3, cv::Scalar(0));
    cv::cvtColor(window, window, cv::COLOR_BGR2RGB);
    cv::namedWindow("Bezier Curve", cv::WINDOW_AUTOSIZE);

    cv::setMouseCallback("Bezier Curve", mouse_handler, nullptr);

    int key = -1;
    while (key != 27)
    {
        for (auto& point : control_points)
        {
            cv::circle(window, point, 3, { 255, 255, 255 }, 3);
        }

        if (control_points.size() == 4)
        {
            //naive_bezier(control_points, window);
            bezier(control_points, window);

            cv::imshow("Bezier Curve", window);
            cv::imwrite("my_bezier_curve.png", window);
            key = cv::waitKey(0);

            return 0;
        }

        cv::imshow("Bezier Curve", window);
        key = cv::waitKey(20);
    }

    return 0;
}
