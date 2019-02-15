#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

void findTemplate(cv::Mat const &frame, cv::Mat const &pattern)
{
    int const rows = frame.rows - pattern.rows + 1;
    int const cols = frame.cols - pattern.cols + 1;
    cv::Mat res(rows, cols, CV_32FC1);
    cv::matchTemplate(frame, pattern, res, cv::TM_CCORR);

    double minVal{ 0.0 };
    double maxVal{ 0.0 };
    cv::Point minLoc;
    cv::Point maxLoc;
    cv::minMaxLoc(res, &minVal, &maxVal, &minLoc, &maxLoc, {});
    cv::Point const matchLoc = maxLoc;

    cv::Mat show;
    frame.copyTo(show);
    cv::rectangle(show, matchLoc, cv::Point{ matchLoc.x + pattern.cols, matchLoc.y + pattern.rows }, cv::Scalar::all(0), 2, 8, 0);
    cv::imshow("Result", show);
}

int main()
{
    cv::Mat const pattern = cv::imread("pattern.png");

    auto capture = cv::VideoCapture("video.mp4");
    cv::Mat frame;
    while (capture.isOpened())
    {
        if (!capture.read(frame) || cv::waitKey(10) == 27)
            break;
        else
            findTemplate(frame, pattern);
    }
    capture.release();
    cv::destroyAllWindows();
    return 0;
}