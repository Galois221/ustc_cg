#include "ImageWidget.h"
#include <QImage>
#include <QPainter>
#include <QtWidgets>
#include <iostream>

using std::cout;
using std::endl;

ImageWidget::ImageWidget()
{
	ptr_image_ = new QImage();
	ptr_image_backup_ = new QImage();
	current_line_ = new QLine();
	draw_status = false;
}


ImageWidget::~ImageWidget()
{
	for (auto& p : pair_array_) 
		if(p)
	    {
			delete p;
	    }
	if (ptr_image_)
		delete ptr_image_;
	if (current_line_) delete current_line_;
}

void ImageWidget::paintEvent(QPaintEvent *paintevent)
{
	QPainter painter;
	painter.begin(this); //开始一个画笔,等价于QPainter painter(this);

	// Draw background
	painter.setBrush(Qt::lightGray);
	QRect back_rect(0, 0, width(), height()); //draw the background
	painter.drawRect(back_rect);

	int left = (width() - ptr_image_->width() * ratio) / 2;  //notice:the valuetype is int
	int top = (height() - ptr_image_->height() * ratio) / 2;

	// Draw image
	QRect rect = QRect( left, top, ptr_image_->width()*ratio, ptr_image_->height()*ratio);
	painter.drawImage(rect, *ptr_image_);  //painter.drawimage函数,用来放置一个图片

	// Draw lines
	for (auto& p : pair_array_)
	{
		QLine temp_line_;
		temp_line_.setP1(QPoint(p->x1()*ratio  + left, p->y1()*ratio  + top));
		temp_line_.setP2(QPoint(p->x2()*ratio  + left, p->y2()*ratio  + top));
		draw_InputLine(temp_line_);
	}

	if (draw_status)
	{
		draw_InputLine(*current_line_);
	}

	painter.end();
}

void ImageWidget::mousePressEvent(QMouseEvent* event){
	if (event->button() == Qt::LeftButton) {
		draw_status = true;
		current_line_ ->setP1(event->pos());
		current_line_->setP2(event->pos());
		update();
	}
}

void ImageWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (draw_status) {
		current_line_->setP2(event->pos());
		update();
	}
}

void ImageWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && draw_status)
	{
		draw_status = false;

		int left=(width() - ptr_image_->width()*ratio) / 2;
		int top= (height() - ptr_image_->height()*ratio) / 2;
		current_line_->setP2(event->pos());
		//需要新的内存，而不能直接添加QLine
		QPoint p1((current_line_->x1() - left), (current_line_->y1() - top));  //the point coordinate is relative to left
		QPoint p2((current_line_->x2() - left), (current_line_->y2() - top));
		QLine* current_pair_ = new QLine();
		current_pair_->setP1(p1);
		current_pair_->setP2(p2);
		pair_array_.push_back(current_pair_);

		update();
	}
}

void ImageWidget::draw_InputLine(const QLine& line) {
	if (!visible) return;
	QPainter painter(this);
	QPen pen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	painter.setPen(pen);
	//画叉叉
	painter.drawLine(line.p1() - QPoint(3, 3), line.p1() + QPoint(3, 3));
	painter.drawLine(line.p1() - QPoint(3, -3), line.p1() + QPoint(3, -3));
	//画线
	painter.drawLine(line.p1(), line.p2());
	//画叉叉
	pen.setColor(Qt::blue);
	painter.setPen(pen);
	painter.drawLine(line.p2() - QPoint(3, 3), line.p2() + QPoint(3, 3));
	painter.drawLine(line.p2() - QPoint(3, -3), line.p2() + QPoint(3, -3));

}

QImage* ImageWidget::Open()
{
	// Open file
	QString fileName = QFileDialog::getOpenFileName(this, tr("Read Image"), ".", tr("Images(*.bmp *.png *.jpg)"));

	// Load file
	if (!fileName.isEmpty())
	{
		ptr_image_->load(fileName);
		*(ptr_image_backup_) = *(ptr_image_); //backup a image
	}

	file_name_ = fileName;

	//ptr_image_->invertPixels(QImage::InvertRgb);   可以查看QImage.h的具体函数
	//*(ptr_image_) = ptr_image_->mirrored(true, true);
	//*(ptr_image_) = ptr_image_->rgbSwapped();
	cout<<"image size: "<<ptr_image_->width()<<' '<<ptr_image_->height()<<endl;
	return ptr_image_;
	update();
}

void ImageWidget::Save()
{
	if (file_name_.isNull()) {
		return;
	}
	ptr_image_->save(file_name_);
}

void ImageWidget::SaveAs()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), ".", tr("Images(*.bmp *.png *.jpg)"));
	if (filename.isNull())
	{
		return;
	}	

	ptr_image_->save(filename);
}

void ImageWidget::Invert()
{
	for (int i=0; i<ptr_image_->width(); i++)
	{
		for (int j=0; j<ptr_image_->height(); j++)
		{
			QRgb color = ptr_image_->pixel(i, j);
			ptr_image_->setPixel(i, j, qRgb(255-qRed(color), 255-qGreen(color), 255-qBlue(color)) );
		}
	}

	// equivalent member function of class QImage
	// ptr_image_->invertPixels(QImage::InvertRgb);
	update();
}

void ImageWidget::Mirror(bool ishorizontal, bool isvertical)
{
	QImage image_tmp(*(ptr_image_)); //generate a tmp_image
	int width = ptr_image_->width();
	int height = ptr_image_->height();

	if (ishorizontal)
	{
		if (isvertical)
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(width-1-i, height-1-j));
				}
			}
		} 
		else			//仅水平翻转			
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(width-1-i, j));
				}
			}
		}
		
	}
	else
	{
		if (isvertical)		//仅垂直翻转
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(i, height-1-j));
				}
			}
		}
	}

	// equivalent member function of class QImage
	//*(ptr_image_) = ptr_image_->mirrored(true, true);
	update();
}

void ImageWidget::TurnGray()
{
	for (int i=0; i<ptr_image_->width(); i++)
	{
		for (int j=0; j<ptr_image_->height(); j++)
		{
			QRgb color = ptr_image_->pixel(i, j);
			int gray_value = (qRed(color)+qGreen(color)+qBlue(color))/3;
			ptr_image_->setPixel(i, j, qRgb(gray_value, gray_value, gray_value) );
		}
	}

	update();
}

void ImageWidget::Restore()
{
	*(ptr_image_) = *(ptr_image_backup_);
	update();
}

void ImageWidget::StartSetting()
{

}

void ImageWidget::ClearSetting()
{
	for (auto& p : pair_array_)
		if (p) delete p;  //若不是空集
	pair_array_.clear();
	draw_status = false;
	update();
}

void ImageWidget::IDWWarping(int setgap)
{
	Warp* t;
	t = new WarpIDW(pair_array_);
	t->imageWarp(ptr_image_, setgap);
	update();
}

void ImageWidget::RBFWarping(int setgap)
{
	Warp* t;
	t = new WarpRBF(pair_array_);
	t->imageWarp(ptr_image_, setgap);
	update();
}

bool ImageWidget::change_points_visible()
{
	visible=(visible == true)?false:true;
	update();
	return visible;
}

void ImageWidget::setratio(int value)
{
	ratio = double(value)/100;
	update();
}