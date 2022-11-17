# OpenCV学习笔记

```c++
cv:: image = cv::imread(fileName.toLatin1().data());
cv:: cvtcolor(image,image,cv::COLOR_BRG2RGB);
cv:: Mat image_save;
cv:: imwrite(filename.toLatin1().data(),image);
cv:: MatIterator_<cv::Vec3b> iter;
image.at<cv::Vec3b> //像素
```

