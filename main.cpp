#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "opencv2/objdetect/objdetect.hpp"

using namespace std;
using namespace cv;

/*variables*/
bool ldown = false, lup = false;

Mat src;
Point corner1, corner2;
Rect box;
Mat local;
//VideoCapture cap(1); para la webcam
/*funciones*/
static void mouse_callback(int event, int x, int y, int, void *);

//FUNCION PRINCIPAL
int main(){
    //VideoCapture cap(0); //Para hacerlo con vidocaptura
    while(true){
        //LEER LA IMAGEN
        //cap>>src; //Para la webcam
        src=imread("C:\\Users\\Usuario\\Desktop\\p.png");
        resize(src,src,Size(780,560)); //Esto pueden ponerle como quieran es solo par ajustar la imagen
        namedWindow("IMAGEN",WINDOW_NORMAL);
        imshow("IMAGEN", src);
        // Set the mouse event callback function
        setMouseCallback("IMAGEN", mouse_callback);
        int c;
        c = waitKey( 1 );
        if( (char)c == 27 )
          { break; }
    }
}

// REGION SELCCIONADAD
// Callback function for mouse events
static void mouse_callback(int event, int x, int y, int, void *){
    //Cuando se presiona el click
    if(event == EVENT_LBUTTONDOWN){
        ldown = true;
        corner1.x = x;
        corner1.y = y;
        //cout << "****** COORDENADAS DEL PRIMER PUNTO ******" << corner1 << endl;
    }

    //Cuando se libera el click
    if(event == EVENT_LBUTTONUP){
        if(abs(x - corner1.x) > 10 && abs(y - corner1.y) > 10){
            lup = true;
            corner2.x = x;
            corner2.y = y;
           // cout << "****** COORDENADAS DEL SEGUNDO PUNTO ******" << corner2 << endl << endl;
        }
        else{
            cout << "Please select a bigger region" << endl;
            ldown = false;
        }
    }

    // Actualiza y recorta la region
    if(ldown == true && lup == false)
    {
        Point pt;
        pt.x = x;
        pt.y = y;
        Mat local_img = src.clone();
        rectangle(local_img, corner1, pt, Scalar(0, 0, 255));
        imshow("IMAGEN", local_img);
    }
    // Define ROI and crop it out when both corners have been selected
    if(ldown == true && lup == true){
        box.width = abs(corner1.x - corner2.x);
        box.height = abs(corner1.y - corner2.y);
        box.x = min(corner1.x, corner2.x);
        box.y = min(corner1.y, corner2.y);

        // Make an image out of just the selected ROI and display it in a new window
        Mat crop(src, box);
        namedWindow("REGION",CV_WINDOW_NORMAL);
        imshow("REGION", crop);
        ldown = false;
        lup = false;

        //OBTENER EL VALOR PROMEDIO RGB
        Scalar promrgb=cv::mean(crop);
        cout << "                                    " << endl;
        cout << "**VALORES PROMEDIO DE LA REGION***" << endl;
        cout << "Valor promedio B = " << promrgb.val[0] << endl;
        cout << "Valor promedio G=  " << promrgb.val[1] << endl;
        cout << "Valor promedio R=  " << promrgb.val[2] << endl;

        //OBTENER EL VALOR PROMEDIO HSV

        Mat imghsv;
        cvtColor(crop, imghsv,CV_BGR2HSV);
        Scalar promhsv=cv::mean(imghsv);
        cout << "                   " << endl;
        cout << "Valor promedio H = " << promhsv.val[0] << endl;
        cout << "Valor promedio S=  " << promhsv.val[1] << endl;
        cout << "Valor promedio V=  " << promhsv.val[2] << endl;

    }
}
