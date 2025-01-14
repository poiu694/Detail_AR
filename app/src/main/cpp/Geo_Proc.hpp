#ifndef Geo_Proc_h
#define Geo_Proc_h

#include "basic.h"

class Geo_Proc
{
    private:
    Mat Ball_templete;
    Mat Ball_and_Sol_templete;
    vector<Point3d> world_table_outside_corners;
    vector<Point3f> world_table_inside_corners_f;
    vector<Point3d> world_table_inside_corners_d;
    vector<Point3d> wor_ball_loc; // sorted
    vector<int> world_ball_color_ref;
    int wor_ball_n;

    Mat INTRINSIC;
    Mat distCoeffs;
    
    Mat rvec, tvec;

    bool device_dir; // true: 가로 false: 세로

    // 실제 당구 시스템의 규격을 저장하는 변수
    int ball_rad;
    int table_depth;
    int edge_thickness;
    int B_W;
    int B_H;

    //Error handling
    Mat e_rvec, e_tvec;
    int e_num, cum;

    // for drawing.
    vector<Point2f> H_img_pts;
    vector<Point2f> H_wor_pts;

    //for debugging
    Mat temp_img;

    public:

    Geo_Proc(int f_len);
    void Set_Device_Dir(bool dir, Mat& img);
    int Find_Balls_3D_Loc(vector<Point2i>& corners, vector<Point2i>& balls_center, vector<int>& ball_color_ref,
        vector<Point2i>& wor_ball_cen, bool update);


    int Find_Cam_Pos(vector<Point2i>& input_corners, vector<Point2i>& balls_center, vector<int>& ball_color_ref);
    bool Cam_Pos_with_Four_Corners(vector<Point2i>& input_corners, Mat& in_rvec, Mat& in_tvec,  bool comp_prev);
    int Cam_Pos_with_Two_Corners_Two_balls(vector<Point2i>& input_corners, vector<Point2i>& balls_center, 
        vector<int>& ball_color_ref, Mat& in_rvec, Mat& in_tvec);
    int Cam_Pos_with_One_Corners_Three_balls(vector<Point2i>& input_corners, vector<Point2i>& balls_center, 
        vector<int>& ball_color_ref, Mat& in_rvec, Mat& in_tvec);
    bool Cam_Pos_with_Four_balls(vector<Point2i>& balls_center, vector<int>& ball_color_ref, Mat& in_rvec, Mat& in_tvec);
    

    void Reprojection_Error(vector<Point3d>& match1, vector<Point2d>& match2,  Mat& rvec_, Mat& tvec_, 
        double& distance);
    bool Error_Comparison_with_Prev_Frame(Mat& in_rvec, Mat& in_tvec, double& distance);
    void Distance_from_Prev_Frame(Mat& rvec_, Mat& tvec_, double& distance);
    int Distance_from_Error_Frame(Mat& rvec_, Mat& tvec_);
        
    Mat GetTemplate(void);
    void SaveTemplate(Mat& templete);
    void Draw_Obj_on_Template();
    void Draw_3D_Template_on_Img(Mat& img);

    // for test
    void Draw_Object(Mat& img);
};

#endif