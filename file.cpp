#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

// Function to convert RGB to Hex
string rgbToHex(Vec3b color) {
    stringstream stream;
    stream << "#" << hex << setfill('0');
    for (int i = 0; i < 3; i++) {
        stream << setw(2) << static_cast<int>(color[i]);
    }
    return stream.str();
}

// Function to visualize the color palette
void visualizeColorPalette(Mat& image, Mat& colors, Mat& percentages) {
    Mat palette(100, 100, CV_8UC3, Scalar(255, 255, 255));

    int width = image.cols / 2;
    for (int i = 0; i < colors.rows; i++) {
        Vec3b color(colors.at<Vec3b>(i));
        string hexColor = rgbToHex(color);
        rectangle(palette, Rect(0, i * 10, width, 10), Scalar(color[0], color[1], color[2]), FILLED);
        putText(palette, hexColor, Point(width + 10, i * 10 + 8), FONT_HERSHEY_SIMPLEX, 0.35, Scalar(0, 0, 0), 1);
    }

    namedWindow("Color Palette", WINDOW_AUTOSIZE);
    imshow("Color Palette", palette);
    waitKey(0);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <image_path>" << endl;
        return -1;
    }

    Mat image = imread(argv[1]);
    if (image.empty()) {
        cerr << "Error: Could not open or find the image." << endl;
        return -1;
    }

    Mat resizedImage;
    resize(image, resizedImage, Size(250, 250));

    Mat img;
    resizedImage.convertTo(img, CV_32F);
    img /= 255.0;

    img = img.reshape(0, resizedImage.cols * resizedImage.rows);

    int k = 10;
    Mat labels, colors;
    kmeans(img, k, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 100, 0.2), 3, KMEANS_PP_CENTERS, colors);

    Mat percentages = Mat::zeros(k, 1, CV_32F);

    for (int i = 0; i < labels.rows; i++) {
        percentages.at<float>(labels.at<int>(i)) += 1.0f;
    }
    percentages /= static_cast<float>(labels.rows);

    cout << "The color palette is:" << endl;
    for (int i = 0; i < k; i++) {
        Vec3b color(colors.at<Vec3b>(i));
        string hexColor = rgbToHex(color);
        cout << "Color " << i << ": " << hexColor << " (RGB: " << color << "), Percentage: " << percentages.at<float>(i) * 100 << "%" << endl;
    }

    visualizeColorPalette(resizedImage, colors, percentages);

    return 0;
}
