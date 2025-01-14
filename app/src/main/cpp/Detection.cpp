#include "Detection.hpp"
#include "Detect_Table_Func.hpp"
#include "Detect_Ball_Func.hpp"

void Detection::Set_Image(Mat& input_img, bool flag){

    img = input_img;

    if(flag) {
        cvtColor(img, img_hsv, COLOR_BGR2HSV);
    }
    else {
        cvtColor(img, img_hsv, COLOR_RGB2HSV); // 안드로이드 에서는 RGB2HSV
        cvtColor(input_img, input_img, COLOR_BGRA2BGR); // 안드로이드는 4채널을 불러오기 때문
    }
}

int Detection::Detect_Billiard_Corners(vector<Point2i>& input_corners){

    Detect_Billiard(img_hsv, blue_and_morph);

    Extract_Biggest_Blob_with_Center(blue_and_morph, Big_blob, Big_blob_center);

    Detect_Billiard_Hole(Big_blob, hole, Big_blob_without_hole);

    Detect_Billiard_Edge(Big_blob_without_hole, Big_blob_center, Candidate_lines);

    Calculation_Billiard_Corner(Candidate_lines, Big_blob_center, corners);

    input_corners = corners;

    int corner_size = corners.size();
    if(corner_size < 0 || corner_size >=5)
        return -1;
    else  
        return corner_size;
}


int Detection::Detect_Billirad_Balls(vector<Point2i>& input_balls_center,  vector<int>& input_ball_color_ref){
    
    Detect_ball_color(img_hsv, ball_colors);
    Match_ball_and_color(ball_colors, hole, ball_candidate, label_with_color);
    // R R Y W 조합만 가능한것도 고려해서 거르자

    Find_ball_center(ball_candidate, label_with_color, ball_colors,balls_center, ball_color_ref);

    int c_t[3]={0,};
    int s = ball_color_ref.size();
    
    if(s>4) // number of balls must be four.
        ball_color_ref.resize(4);
    
    for(int i=0; i<s ; i++)
        c_t[ball_color_ref[i]]++;
     
    if(!(c_t[0]<=2 && c_t[1] <=1 && c_t[2] <=1)) // Red Red Yellow White.
        return -1;

    input_balls_center = balls_center;
    input_ball_color_ref = ball_color_ref;

    int ball_num = balls_center.size();

    return ball_num;
}



void Detection::Clear_prev_frame_info(){
    corners.clear();
    ball_candidate.clear();
    label_with_color.clear();
    Candidate_lines.clear();
    balls_center.clear();
    ball_color_ref.clear();
}

void Detection::Draw_Corners(Mat& img, vector<Point2i>& input_corners){
    int size = input_corners.size();
    for(int j=0; j< size ;j++){
        circle(img, Point(input_corners[j].x, input_corners[j].y), 50, Scalar(0, j*80, 0), 2, 8, 0);
    }

}


void Detection::Draw_Balls(Mat& img, vector<Point2i>& input_balls_center,  vector<int>& input_ball_color_ref){
    int size2 = input_balls_center.size();
    for(int j=0; j<size2 ; j++){
        int x = input_balls_center[j].x;
        int y = input_balls_center[j].y;
        circle(img, Point(x, y), 4, Scalar(0, 255, 0), 2);
        int color = input_ball_color_ref[j];

        if(color==0){
            putText(img, "RED", Point(x,y), 1,1.5, Scalar(0,255,0), 2,8);
        }
        else if(color==1){
            putText(img, "Yellow", Point(x,y), 1,1.5, Scalar(0,255,0), 2,8);
        }
        else
        {
            putText(img, "White", Point(x,y), 1,1.5, Scalar(0,255,0), 2,8);
        }
    }
}