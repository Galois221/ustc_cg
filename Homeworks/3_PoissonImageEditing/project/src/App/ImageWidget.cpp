#include "ImageWidget.h"
#include <QImage>
#include <QPainter>
#include <QtWidgets> 
#include <iostream>
#include "ChildWindow.h"
#include "Rect.h"
#include "Polygon.h"
#include "Freehand.h"
#include "Poisson.h"
#include "ScanLine.h"
#include <vector>

using namespace std;

ImageWidget::ImageWidget(ChildWindow* relatewindow)
{
	//image_ = new QImage();
	//image_backup_ = new QImage();

	draw_status_ = kNone;
	is_choosing_ = false;
	is_pasting_ = false;
	polygoning = false;
	copied = false;
	mixed = false;

	point_start_ = QPoint(0, 0);
	point_end_ = QPoint(0, 0);

	source_window_ = NULL;
	figure = NULL;
	solver = NULL;
}

ImageWidget::~ImageWidget(void)
{
	if (figure)
	{
		delete figure;
		figure = NULL;
	}
	if (solver)
	{
		delete solver;
		solver = NULL;
	}
}

int ImageWidget::ImageWidth()
{
	return image_mat_.cols;
}

int ImageWidget::ImageHeight()
{
	return image_mat_.rows;
}

void ImageWidget::set_draw_status_to_Rect()
{
	if (polygoning)
	{
		QMessageBox::about(this, tr("Error"), tr("Finish your Polygon !!!"));
		return;
	}
	draw_status_ = kRect;
	if (figure != NULL)
	{
		delete figure;
		figure = NULL;
	}
}

void ImageWidget::set_draw_status_to_Poly()
{
	if (polygoning)
	{
		QMessageBox::about(this, tr("Error"), tr("Finish your Polygon !!!"));
		return;
	}

	draw_status_ = kPoly;
	if (figure != NULL)
	{
		delete figure;
		figure = NULL;
	}
}

void ImageWidget::set_draw_status_to_Free()
{
	if (polygoning)
	{
		QMessageBox::about(this, tr("Error"), tr("Finish your Polygon !!!"));
		return;
	}
	draw_status_ = kFree;
	if (figure != NULL)
	{
		delete figure;
		figure = NULL;
	}
}

void ImageWidget::set_draw_status_to_paste()
{
	if (polygoning)
	{
		QMessageBox::about(this, tr("Error"), tr("Finish your Polygon !!!"));
		return;
	}
	image_mat_undo_ = image_mat_.clone();
	draw_status_ = kPaste;
	setMouseTracking(true);
	mixed = false;
}

void ImageWidget::set_draw_status_to_paste_mix()
{
	if (polygoning)
	{
		QMessageBox::about(this, tr("Error"), tr("Finish your Polygon !!!"));
		return;
	}
	image_mat_undo_ = image_mat_.clone();			// for dragging
	draw_status_ = kPaste;
	setMouseTracking(true);
	mixed = true;
}

void ImageWidget::set_source_window(ChildWindow* childwindow)
{
	source_window_ = childwindow;
}

void ImageWidget::Copy()
{
	if (polygoning)
	{
		QMessageBox::about(this, tr("Error"), tr("Finish your Polygon !!!"));
		return;
	}
	if (figure == NULL)
	{
		QMessageBox::about(this, tr("ERROR"), tr("Please Choose first"));
		return;
	}
	if (solver)
	{
		delete solver;
	}
	int x1, y1, x2, y2;
	if (figure->points.empty()) {
		x1 = point_start_.rx();
		y1 = point_start_.ry();
		x2 = point_end_.rx();
		y2 = point_end_.ry();
		//let (x1,y1) is the start point , (x2,y2) is the end point
		if (x1 > x2) {
			int tmp = x1;
			x1 = x2;
			x2 = tmp;
		}
		if (y1 > y2) {
			int tmp = y1;
			y1 = y2;
			y2 = tmp;
		}
		selected = MatrixXd::Zero( y2 - y1 + 1, x2 - x1 + 1);
		for (int i = 1; i < y2 - y1; i++)
			for (int j = 1; j < x2 - x1; j++)
				selected(i, j) = 1;
	}
	else
	{
		ScanLine scan;
		//frame with a rectangle
		x1 = x2 = figure->points[0].rx();
		y1 = y2 = figure->points[0].ry();
		vector<int> x_points;  //pick up the vertex coordinate
		vector<int> y_points;
		
		for (int k = 0; k < figure->points.size(); k++)
		{
			if (figure->points[k].rx() < x1)x1 = figure->points[k].rx();
			if (figure->points[k].ry() < y1)y1 = figure->points[k].ry();
			if (figure->points[k].rx() > x2)x2 = figure->points[k].rx();
			if (figure->points[k].ry() > y2)y2 = figure->points[k].ry();
		}
		for (int k = 0; k < figure->points.size(); k++)
		{
			x_points.push_back(figure->points[k].rx() - x1);
			y_points.push_back(figure->points[k].ry() - y1);
		}
		//set the selected
		selected = scan.Scan2Mat(y2 - y1 + 1, x2 - x1 + 1, x_points, y_points);
	}
	cout << "select : " << y2 - y1 + 1 << "      " << x2 - x1 + 1 << endl;
	pos_x = x1;
	pos_y = y1;
	width_select = x2 - x1 + 1;
	height_select = y2 - y1 + 1;
	Poisson poisson;
	solver = poisson.Get_Poisson_solver(selected);
	copied = true;
}

void ImageWidget::paintEvent(QPaintEvent* paintevent)
{
	QPainter painter;
	painter.begin(this);

	// Draw background
	painter.setBrush(Qt::lightGray);
	QRect back_rect(0, 0, width(), height());
	painter.drawRect(back_rect);

	// Draw image
	QImage image_show = QImage((unsigned char*)(image_mat_.data), image_mat_.cols, image_mat_.rows, image_mat_.step, QImage::Format_RGB888);
	QRect rect = QRect(0, 0, image_show.width(), image_show.height());
	painter.drawImage(rect, image_show);

	// Draw choose region
	painter.setBrush(Qt::NoBrush);
	painter.setPen(Qt::red);

	if (figure != NULL) figure->Draw(painter);

	painter.end();
}

void ImageWidget::mousePressEvent(QMouseEvent* mouseevent)
{
	if (Qt::LeftButton == mouseevent->button())
	{
		switch (draw_status_)
		{
		case kRect:
			if (figure != NULL)
				delete figure;
			is_choosing_ = true;
			figure = new Rect();
			point_start_ = point_end_ = mouseevent->pos();
			figure->set_start_point_(point_start_);
			break;
		case kPoly:
			if (figure == NULL) {
				point_start_ = point_end_ = mouseevent->pos();
				figure = new Polygonn();
				polygoning = true;
				is_choosing_ = true;
				figure->set_start_point_(point_start_);
				figure->set_end_point_(point_end_);
				figure->points.push_back(point_start_);
			}
			else
			{
				point_end_ = mouseevent->pos();
				figure->set_start_point_(point_start_);
				figure->set_end_point_(point_end_);
				figure->points.push_back(point_end_);
			}
			setMouseTracking(true);
			break;
		case kFree:
			if (figure != NULL)
				delete figure;
			is_choosing_ = true;
			point_start_ = point_end_ = mouseevent->pos();
			figure = new Freehand();
			figure->points.push_back(point_start_);
			break;
		case kPaste:
		{
			is_pasting_ = true;

			if (!source_window_->imagewidget_->copied || source_window_ == NULL)
			{
				QMessageBox::about(this, tr("ERROR"), tr("Please copy first"));
				setMouseTracking(false);
				return;
			}

			// Start point in object image
			int xpos = mouseevent->pos().rx();
			int ypos = mouseevent->pos().ry();

			// Start point in source image
			int xsourcepos = source_window_->imagewidget_->pos_x;
			int ysourcepos = source_window_->imagewidget_->pos_y;
			cout << source_window_->imagewidget_->height() << " " << source_window_->imagewidget_->width() << endl;

			// Width and Height of rectangle region
			int w = source_window_->imagewidget_->width_select;
			int h = source_window_->imagewidget_->height_select;

			//make sure the graph set ture palaces
			if (xpos + w / 2 >= width())xpos = width() - w / 2 - 1;
			if (ypos + h / 2 >= height())ypos = height() - h / 2 - 1;
			if (xpos - w + w / 2 < 0)xpos = w - w / 2;
			if (ypos - h + h / 2 < 0)ypos = h - h / 2;

			if (xpos + w / 2 >= width() || ypos + h / 2 >= height() || xpos - w + w / 2 < 0 || ypos - h + h / 2 < 0)
			{
				QMessageBox::about(this, tr("ERROR"), tr("Cannot put it here!"));
			}

			MatrixXd selected = source_window_->imagewidget_->selected;

			// Paste
			Poisson poisson;
			Solver* source_solver = source_window_->imagewidget_->solver;
			MatrixXd f, g;
			for (int k = 0; k < 3; k++)
			{
				f = MatrixXd::Zero(h, w);
				g = MatrixXd::Zero(h, w);
				for (int i = 0; i < h; i++) {
					for (int j = 0; j < w; j++) {
						f(i, j) = image_mat_.at<cv::Vec3b>(i + ypos - h + h / 2, j + xpos - w + w / 2)[k];
						g(i, j) = source_window_->imagewidget_->image().at<cv::Vec3b>(i + ysourcepos, j + xsourcepos)[k];
					}
				}


				VectorXd solve = poisson.Poisson_solver(f, g, selected, source_solver, mixed);
				for (int i = 0; i < h; i++) {
					for (int j = 0; j < w; j++) {
						double tmp = solve(i * w + j);
						if (tmp > 255)tmp = 255;
						if (tmp < 0)tmp = 0;
						image_mat_.at<cv::Vec3b>(i + ypos - h + h / 2, j + xpos - w + w / 2)[k] = floor(tmp);
					}
				}
			}
		}

		update();
		break;

		default:
			break;
		}
	}
	if (Qt::RightButton == mouseevent->button())
	{
		switch (draw_status_)
		{
		case kRect:
			break;
		case kPoly:
			figure->points.push_back(point_end_);
			figure->points.push_back(point_start_);
			polygoning = false;
			is_choosing_ = true;
			draw_status_ = kNone;
			break;
		default:
			break;
		}
	}
}

cv::Mat ImageWidget::image()
{
	return image_mat_;
}

void ImageWidget::mouseMoveEvent(QMouseEvent* mouseevent)
{
	switch (draw_status_)
	{
	case kRect:
		// Store point position for rectangle region
		if (is_choosing_)
		{
			point_end_ = mouseevent->pos();
			figure->set_end_point_(point_end_);
		}
		break;
	case kPoly:
		// Store point position for polygon region
		if (is_choosing_)
		{
			point_end_ = mouseevent->pos();
			figure->set_end_point_(point_end_);
		}
		break;
	case kFree:
		// Store point position for freehand region
		if (is_choosing_)
		{
			point_end_ = mouseevent->pos();
			figure->points.push_back(point_end_);
		}
		break;

	case kPaste:
		// Paste rectangle region to object image
		if (is_pasting_)
		{
			is_pasting_ = true;

			// Start point in object image
			int xpos = mouseevent->pos().rx();
			int ypos = mouseevent->pos().ry();

			// Start point in source image
			int xsourcepos = source_window_->imagewidget_->pos_x;
			int ysourcepos = source_window_->imagewidget_->pos_y;

			// Width and Height of rectangle region
			int w = source_window_->imagewidget_->width_select;
			int h = source_window_->imagewidget_->height_select;

			//make sure the graph set ture palaces
			if (xpos + w / 2 >= width())xpos = width() - w / 2 - 1;
			if (ypos + h / 2 >= height())ypos = height() - h / 2 - 1;
			if (xpos - w + w / 2 < 0)xpos = w - w / 2;
			if (ypos - h + h / 2 < 0)ypos = h - h / 2;

			if (xpos + w / 2 >= width() || ypos + h / 2 >= height() || xpos - w + w / 2 < 0 || ypos - h + h / 2 < 0)
			{
				QMessageBox::about(this, tr("ERROR"), tr("Cannot put it here!"));
			}

			image_mat_ = image_mat_undo_.clone();
			MatrixXd selected = source_window_->imagewidget_->selected;

			// Paste
			Poisson poisson;
			Solver* source_solver = source_window_->imagewidget_->solver;
			MatrixXd f, g;
			for (int k = 0; k < 3; k++)
			{
				f = MatrixXd::Zero(h, w);
				g = MatrixXd::Zero(h, w);
				for (int i = 0; i < h; i++) {
					for (int j = 0; j < w; j++) {
						f(i, j) = image_mat_.at<cv::Vec3b>(i + ypos - h + h / 2, j + xpos - w + w / 2)[k];
						g(i, j) = source_window_->imagewidget_->image().at<cv::Vec3b>(i + ysourcepos, j + xsourcepos)[k];
					}
				}


				VectorXd solve = poisson.Poisson_solver(f, g, selected, source_solver, mixed);
				for (int i = 0; i < h; i++) {
					for (int j = 0; j < w; j++) {
						double tmp = solve(i * w + j);
						if (tmp > 255)tmp = 255;
						if (tmp < 0)tmp = 0;
						image_mat_.at<cv::Vec3b>(i + ypos - h + h / 2, j + xpos - w + w / 2)[k] = floor(tmp);
					}
				}
			}
		}

	default:
		break;
	}
	update();
}

void ImageWidget::mouseReleaseEvent(QMouseEvent* mouseevent)
{
	switch (draw_status_)
	{
	case kRect:
		if (is_choosing_)
		{
			point_end_ = mouseevent->pos();
			figure->set_end_point_(point_end_);
			is_choosing_ = false;
			draw_status_ = kNone;
		}
		break;
	case kPoly:
		if (is_choosing_)
		{
			point_end_ = mouseevent->pos();
			figure->set_end_point_(point_end_);
		}
		break;
	case kFree:
		if (is_choosing_)
		{
			point_end_ = mouseevent->pos();
			figure->points.push_back(point_end_);
			figure->points.push_back(point_start_);
			is_choosing_ = false;
			draw_status_ = kNone;
		}
		break;
	case kPaste:
		if (is_pasting_)
		{
			delete source_window_->imagewidget_->figure;
			source_window_->imagewidget_->figure = NULL;
			is_pasting_ = false;
			draw_status_ = kNone;
		}
		break;
	default:
		break;
	}

	update();
}

void ImageWidget::Open(QString filename)
{
	// Load file
	if (!filename.isEmpty())
	{
		image_mat_ = cv::imread(filename.toLatin1().data());
		cv::cvtColor(image_mat_, image_mat_, cv::COLOR_BGR2RGB);//transform bgr to rgb
		image_mat_backup_ = image_mat_.clone();
	}

	//	setFixedSize(image_->width(), image_->height());
	//	relate_window_->setWindowFlags(Qt::Dialog);
	//	relate_window_->setFixedSize(QSize(image_->width(), image_->height()));
	//	relate_window_->setWindowFlags(Qt::SubWindow);

		//image_->invertPixels(QImage::InvertRgb);
		//*(image_) = image_->mirrored(true, true);
		//*(image_) = image_->rgbSwapped();
	cout << "image size: " << image_mat_.rows << ' ' << image_mat_.cols << endl;
	update();
}

void ImageWidget::Save()
{
	SaveAs();
}

void ImageWidget::SaveAs()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), ".", tr("Images(*.bmp *.png *.jpg)"));
	if (filename.isNull())
	{
		return;
	}

	cv::Mat image_save;
	cv::cvtColor(image_mat_, image_save, cv::COLOR_BGR2RGB);
	cv::imwrite(filename.toLatin1().data(), image_save);
}

void ImageWidget::Invert()
{
	if (image_mat_.empty())
		return;
	cv::MatIterator_<cv::Vec3b> iter, iterend;
	for (iter = image_mat_.begin<cv::Vec3b>(), iterend = image_mat_.end<cv::Vec3b>(); iter != iterend; ++iter)
	{
		(*iter)[0] = 255 - (*iter)[0];
		(*iter)[1] = 255 - (*iter)[1];
		(*iter)[2] = 255 - (*iter)[2];
	}

	update();
}

void ImageWidget::Mirror(bool ishorizontal, bool isvertical)
{
	if (image_mat_.empty())
		return;
	int width = image_mat_.cols;
	int height = image_mat_.rows;

	if (ishorizontal)
	{
		if (isvertical)
		{
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					image_mat_.at<cv::Vec3b>(j, i) = image_mat_backup_.at<cv::Vec3b>(height - 1 - j, width - 1 - i);
				}
			}
		}
		else
		{
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					image_mat_.at<cv::Vec3b>(j, i) = image_mat_backup_.at<cv::Vec3b>(j, width - 1 - i);
				}
			}
		}

	}
	else
	{
		if (isvertical)
		{
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					image_mat_.at<cv::Vec3b>(j, i) = image_mat_backup_.at<cv::Vec3b>(height - 1 - j, i);
				}
			}
		}
	}

	update();
}

void ImageWidget::TurnGray()
{
	if (image_mat_.empty())
		return;
	cv::MatIterator_<cv::Vec3b> iter, iterend;
	for (iter = image_mat_.begin<cv::Vec3b>(), iterend = image_mat_.end<cv::Vec3b>(); iter != iterend; ++iter)
	{
		int itmp = ((*iter)[0] + (*iter)[1] + (*iter)[2]) / 3;
		(*iter)[0] = itmp;
		(*iter)[1] = itmp;
		(*iter)[2] = itmp;
	}

	update();
}

void ImageWidget::Restore()
{
	image_mat_ = image_mat_backup_.clone();
	update();
}
