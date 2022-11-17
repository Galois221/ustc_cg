#pragma once
#include <QWidget>
#include"WarpIDW.h"
#include"WarpRBF.h"

QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
QT_END_NAMESPACE

class ImageWidget :
	public QWidget
{
	Q_OBJECT

public:
	ImageWidget(void);
	~ImageWidget(void);

protected:
	void paintEvent(QPaintEvent *paintevent);

private:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void draw_InputLine(const QLine& Line);

public slots:
	// File IO
	QImage* Open();												// Open an image file, support ".bmp, .png, .jpg" format
	void Save();												// Save image to current file
	void SaveAs();												// Save image to another file

	// Image processing
	void Invert();												// Invert pixel value in image
	void Mirror(bool horizontal=false, bool vertical=true);		// Mirror image vertically or horizontally
	void TurnGray();											// Turn image to gray-scale map
	void Restore();												// Restore image to origin
	void StartSetting();
	void ClearSetting();
	bool change_points_visible();
	void IDWWarping(int setgap = 0);
	void RBFWarping(int setgap = 0);
	void setratio(int value);         //set the image ratio

private:
	QImage		*ptr_image_;				// image 
	QImage		*ptr_image_backup_;         // backup a image to restore
	QString      file_name_;                //file
	bool         draw_status;
	QLine*       current_line_;
	QVector<QLine*> pair_array_;            //store pair_array_;
	bool visible=true;       //the state of points
	double ratio=1.0;
};

