#include <opencv2\highgui\highgui.hpp>


class ImageFrameConfig {

private:

	cv::Mat matrixND;

	double correlation;
	double chi_sqt;
	double intersect;
	double bhattacharyya;


public:

	ImageFrameConfig();
	~ImageFrameConfig();

	void setCorrelation(double c);
	double getCorrelation();

	void setChiSqt(double);
	double getChiSqt();

	void setIntersect(double);
	double getIntersect();

	void setBhattacharyya(double);
	double getBhattacharyya();

	void setMatrixND(cv::Mat);
	cv::Mat getMatrixND();
};

