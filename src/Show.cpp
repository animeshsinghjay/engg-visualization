#include <math.h>
#include <QtCore>
#include <QtGui>
#include "../include/Three_D_to_Two_D.h"
#include <string>
#include "../include/Show.h"
#include "../include/ngraph.hpp"

using namespace NGraph;
using namespace Qt;


Graph_Imp Show::set_acc_to_ranges(Graph_Imp g, int mode){
   double pos_x, pos_y;
   if (mode == 0)
   {
      pos_x = 350;
      pos_y = 350;
   }
   else if (mode == 1){
      for (int i = 0; i < g.vertices.size(); ++i)
      {
         Triplet temp = {g.vertices[i].one, g.vertices[i].three, 0};
         g.vertices[i] = temp;
      }
      pos_x = 350;
      pos_y = 0;
   }
   else if (mode == 2)
   {
      for (int i = 0; i < g.vertices.size(); ++i)
      {
         Triplet temp = {g.vertices[i].three, g.vertices[i].two, 0};
         g.vertices[i] = temp;
      }
      pos_x = 0;
      pos_y = 350;
   }
   else if (mode == 3){
      pos_x = 0;
      pos_y = 0;
   }
   Graph_Imp ret;
   double min_x = g.vertices[0].one;
   double max_x = g.vertices[0].one;
   double range_x;
   for (int i = 0; i < g.vertices.size(); ++i)
   {
      if (g.vertices[i].one > max_x){
         max_x = g.vertices[i].one;
      }
      if (g.vertices[i].one < min_x){
         min_x = g.vertices[i].one;
      }
   }
   range_x = max_x - min_x;
   double factor_x;
   if (range_x == 0) factor_x = 0;
   else factor_x = 250 / range_x;
   double shift_x = pos_x + 50 - min_x * factor_x;
   for (int i = 0; i < g.vertices.size(); ++i)
   {
      Triplet temp ={0,0,0};
      temp.one = shift_x + g.vertices[i].one * factor_x;
      ret.vertices.push_back(temp);         
   }
   double min_y = g.vertices[0].two;
   double max_y = g.vertices[0].two;
   double range_y;
   for (int i = 0; i < g.vertices.size(); ++i)
   {
      if (g.vertices[i].two > max_y){
         max_y = g.vertices[i].two;
      }
      if (g.vertices[i].two < min_y){
         min_y = g.vertices[i].two;
      }
   }
   range_y = max_y - min_y;
   if (range_y != 0){
      double factor_y = 250 / range_y;
      double shift_y = pos_y + 50 - min_y * factor_y;
      for (int i = 0; i < g.vertices.size(); ++i)
      {
         ret.vertices[i].two = shift_y + g.vertices[i].two * factor_y;
      }
   } 
   ret.edges = g.edges;
   return ret; 
}




bool Show::findEdgeInVector(Edge a, std::vector<Edge> hidden){
   for (int i = 0; i < hidden.size(); ++i)
   {
      if ((a.src == hidden[i].src && a.dest == hidden[i].dest) || (a.dest == hidden[i].src && a.src == hidden[i].dest)){
         return true;
      }
   }
   return false;
}




void Show::drawGraph(Graph_Imp g, QPainter &p, std::vector<Edge> hidden, bool hide){
   for (int i = 0; i < g.vertices.size(); ++i)
   {
      Graph::vertex_set Si = g.edges.out_neighbors(i);
      for (Graph::vertex_set::const_iterator out_vertex = Si.begin(); out_vertex != Si.end(); ++out_vertex)
      {
         Edge temp = {i,*out_vertex};
         if (findEdgeInVector(temp, hidden) && hide){
            p.setPen(QPen(Qt::black, 1, Qt::DotLine, Qt::RoundCap));
            p.drawLine((g.vertices[i].one),(g.vertices[i].two),(g.vertices[*out_vertex].one),(g.vertices[*out_vertex].two));
            p.setPen(QPen(Qt::black, 2));
         }
         else{
            p.drawLine((g.vertices[i].one),(g.vertices[i].two),(g.vertices[*out_vertex].one),(g.vertices[*out_vertex].two));
         }  
      }
   }
}




int Show::show_qt_projections(Three_D_to_Two_D &T, Triplet topdir)
{
   // Three_D_to_Two_D T;
   Graph_Imp G_xy_scaled, G_yz_scaled, G_zx_scaled, G_iso_scaled;
   // T.G = T.toGraph(file);
   T.rotatedG.vertices = T.rotate_vector(T.G.vertices, topdir);
   T.rotatedG.edges = T.G.edges;
   T.projected_xy = T.Projectionxy(T.rotatedG);
   T.projected_yz = T.Projectionyz(T.rotatedG);
   T.projected_zx = T.Projectionzx(T.rotatedG);
   T.projected_isometric = T.Projection_isometric(T.G);

   G_iso_scaled = set_acc_to_ranges(T.projected_isometric, 3);
   G_xy_scaled = set_acc_to_ranges(T.projected_xy, 0);
   G_yz_scaled = set_acc_to_ranges(T.projected_yz, 2);
   G_zx_scaled = set_acc_to_ranges(T.projected_zx, 1);
   // cout<<G3.edges;
   // for (int i = 0; i < G3.vertices.size(); ++i)
   // {
   //    cout<<G3.vertices[i].one<<" "<<G3.vertices[i].two<<" "<<G3.vertices[i].three<<endl;  
   // }
   QApplication a(NULL, NULL);
   QLabel l;
   QPicture pi;
   QPainter p(&pi);

   p.setRenderHint(QPainter::Antialiasing);
   // p.setPen(QPen(Qt::black, 5, Qt::DashDotLine, Qt::RoundCap));

   
   p.eraseRect(0,0,700,700);
   p.setPen(QPen(Qt::black, 2));
   p.drawLine(0,0,WIDTH, 0);
   p.drawLine(WIDTH, 0, WIDTH, HEIGHT);
   p.drawLine(WIDTH, HEIGHT, 0, HEIGHT);
   p.drawLine(0, HEIGHT, 0, 0);
   p.drawLine(WIDTH/2,0,WIDTH/2,HEIGHT);
   p.drawLine(0,HEIGHT/2,WIDTH,HEIGHT/2);
   // for (int i = 0; i < G3.vertices.size(); ++i)
   // {
   //    Graph::vertex_set Si = G3.edges.out_neighbors(i);
   //    for (Graph::vertex_set::const_iterator out_vertex = Si.begin(); out_vertex != Si.end(); ++out_vertex)
   //    {
   //       p.drawLine((G3.vertices[i].one),(G3.vertices[i].two),(G3.vertices[*out_vertex].one),(G3.vertices[*out_vertex].two));
   //    }
   // }

 //   for (int i = 1; i < SIZE; ++i){
   //    x = i*STEP;
   //    y = sin(x);
   //    p.drawLine(FACTOR*prev_x, FACTOR*prev_y, FACTOR*x, FACTOR*y);

   //    prev_x = x;
   //    prev_y = y;
   // }
   drawGraph(G_xy_scaled, p, T.hidden_xy, true);
   drawGraph(G_yz_scaled, p, T.hidden_yz, true);
   drawGraph(G_zx_scaled, p, T.hidden_zx, true);
   drawGraph(G_iso_scaled, p, T.hidden_isometric, true);

   p.setPen(QPen(Qt::black, 0.5, Qt::DotLine, Qt::RoundCap));
   for (int i = 0; i < G_xy_scaled.vertices.size(); ++i)
   {
         p.drawLine(G_xy_scaled.vertices[i].one, G_xy_scaled.vertices[i].two, G_yz_scaled.vertices[i].one, G_yz_scaled.vertices[i].two);
         p.drawLine(G_xy_scaled.vertices[i].one, G_xy_scaled.vertices[i].two, G_zx_scaled.vertices[i].one, G_zx_scaled.vertices[i].two);
   }
   p.end(); // Don't forget this line!

   l.setPicture(pi);
   l.setFixedWidth(700);
   l.setFixedHeight(700);
   l.show();
   return a.exec();
}



// int main(int argc, char *argv[])
// {
//    Triplet topdir = {0, 0, 1.0};
//    try_it("/home/rajas/Desktop/cop290/project/input.txt", topdir);
//    return 0;
// }