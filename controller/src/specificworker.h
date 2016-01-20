/*
 *    Copyright (C) 2015 by YOUR NAME HERE
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
       \brief
       @author authorname
*/







#ifndef SPECIFICWORKER_H
#define SPECIFICWORKER_H

#include <genericworker.h>
#include <innermodel/innermodel.h>
#include <myqtimer.h>
#include <qt4/QtCore/qmap.h>
#include <qt4/QtCore/qmutex.h>
#include <qt4/QtGui/qplaintextedit.h>
#include <qt4/Qt/qdebug.h>
#include <qt4/Qt/qvarlengtharray.h>
#include <lemon/list_graph.h>
#include <lemon/dijkstra.h>
#include <qt4/QtCore/QStack>
#include <sstream>
// #include <curses.h>


using namespace lemon;

const double EulerC=std::exp(1.0);
class SpecificWorker : public GenericWorker
{
Q_OBJECT
typedef ListGraph Graph;
typedef Graph::Node Node;
typedef Graph::Edge Edge;
typedef Graph::EdgeMap<float> LengthMap;
typedef Graph::NodeIt NodeIt;
typedef Graph::NodeMap<QVec> MapQVec;
typedef Graph::NodeMap<int> Mapint;
typedef lemon::Path<Graph::Graph> Path;

#define ROBOT_SIZE 470.f
#define ROBOT_RADIUS 200.f

public:
	SpecificWorker(MapPrx& mprx);	
	~SpecificWorker();
	bool setParams(RoboCompCommonBehavior::ParameterList params);
public slots:
	void compute();
	void reloj();
	void iniciar();
	void parar();
private:
//=====================Variables==================
	bool startbutton;	//BOTON DE STARTINICIADO
	MyQTimer clk;
	QGraphicsScene *scene;	//GRAFICOS DEL ROBOT
	QGraphicsEllipseItem *circulo;
	
	enum class State  { INIT, MAPEAR, HELLEGADO,GOTOPOINTS,CHECKPOINT} ;  
	State st;
	
	int cota, mldata, distsecurity;
	float muestreolaser;
	TBaseState state;		//estado del robot
	QStack<Node> pila;		//camino hasta un nodo
	
	RoboCompLaser::TLaserData ldata, ldatacota, ldatasinord;		//laser
	InnerModel *inner;
	Graph Grafo;
	MapQVec *map;
	LengthMap *cost;
	Node robotnode;
	
	QVec anterior, n, pick;
	
	
	

	//=====================Funciones================== 
	void dijkstra(Node destino);
	void iralpuntomascercano();
	void anadirnodo(QVec nuevo);
	State mapear();
	State checkpoint();
	State goto_point();
	State hellegado();
	bool esquina();
	void accionEsquina();
	void setPick(const Pick &myPick);
	void writeinfo(string _info);
	void pintarnodo(QVec origen, QVec destino);
	void pintardestino(QVec destino);
	bool puntodentrocampolaser(int &pos,float angle, int distpoint);
	void borrarcirculo();
	void pintarposerobot();
	void pintarrobot();
};
#endif

