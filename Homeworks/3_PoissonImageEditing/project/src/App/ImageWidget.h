#pragma once
#include <QWidget>
#include <Eigen/Dense>
#include <Figure.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <Eigen\Sparse>
#include <Eigen\Dense>

class ChildWindow;
QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
QT_END_NAMESPACE

using namespace Eigen;
typedef Eigen::SparseMatrix<double> SparseMatrixType;
typedef Eigen::Triplet<double> T;
typedef Eigen::SimplicialCholesky<SparseMatrixType> Solver;

enum DrawStatus
{
	kRect,
	kPoly,
	kFree,
	kPaste, 
	kNone
};

class ImageWidget :
	public QWidget
{
	Q_OBJECT

public:
	ImageWidget(ChildWindow *relatewindow);
	~ImageWidget(void);

	int ImageWidth();											// Width of image
	int ImageHeight();											// Height of image
	void set_draw_status_to_Rect();
	void set_draw_status_to_Poly();
	void set_draw_status_to_Free();
	void set_draw_status_to_paste();
	void set_draw_status_to_paste_mix();
	void set_source_window(ChildWindow* childwindow);
	cv::Mat image();

protected:
	void paintEvent(QPaintEvent *paintevent);
	void mousePressEvent(QMouseEvent *mouseevent);
	void mouseMoveEvent(QMouseEvent *mouseevent);
	void mouseReleaseEvent(QMouseEvent *mouseevent);

public slots:
	// File IO
	void Open(QString filename);								// Open an image file, support ".bmp, .png, .jpg" format
	void Save();												// Save image to current file
	void SaveAs();												// Save image to another file

	// Image processing
	void Invert();												// Invert pixel value in image
	void Mirror(bool horizontal=false, bool vertical=true);		// Mirror image vertically or horizontally
	void TurnGray();											// Turn image to gray-scale map
	void Restore();												// Restore image to origin
	void Copy();                                                // Selected points to achieve matrix pre-decomposition

public:
	QPoint						point_start_;					// Left top point of rectangle region
	QPoint						point_end_;						// Right bottom point of rectangle region
	int pos_x, pos_y;
	int width_select, height_select;

private:
	cv::Mat image_mat_;						 
	cv::Mat image_mat_backup_;
	cv::Mat image_mat_undo_;

	// Pointer of child window
	ChildWindow					*source_window_;				// Source child window

	// Signs
	DrawStatus					draw_status_;					// Enum type of draw status
	bool						is_choosing_;
	bool						is_pasting_;
	bool                        polygoning;                      //the status of choose
	bool                        copied;                          //the copy achieve or not  
	bool                        mixed;
	MatrixXd                    selected;
	Figure                      * figure;
	Solver* solver;
};

