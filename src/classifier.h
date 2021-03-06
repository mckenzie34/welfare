#include <vl/kdtree.h>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

class Classifier{
	public:
		Classifier();
		int classify(Mat img, int num, int* ids, int* similarities);
		float* encodeImg(Mat img);
		void setDebug(bool isdebug);
		int getCodeDimension();

	private:
		//param
		int descrDimension;
		int frameDimension;
		int totalNumFrames;
		int pcaDimension;
		int geoDimension;
		int numWords;
		int vladDimension;
		int numClass;
		int maxNumFrames;
		bool debug;
		
		//model
		float** w;
		float* b;
		float* encoderWords;
		float* encoderCenter;
		float** encoderProjection;
		VlKDForest* forest;
		
		//operation
		void buildKDForest();
		pair<vector<float>, vector<float> > featureExtraction(const Mat& img);
		void norm(vector<float>& src, int descrSize, int numFrames);
		void normFrame(vector<float>& frames);
		void renorm(vector<float>& descrps);
		vector<float> project(vector<float>& descrs);
		float* encode(vector<float> descrs, vector<float> frames);
		int search(float* code, int num, int* ids, int* similarities);
};
