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
#include "specificworker.h"
#include <qt4/Qt/qdebug.h>

/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)
{
	state.state="IDLE";
	st=State::IDLE;
	stgo=statego::ORIENTARSE;
	inner = new InnerModel("/home/ivan/robocomp/files/innermodel/simpleworld.xml");
	ldata=laser_proxy->getLaserData();
	muestreolaser=(ldata[51].angle-ldata[50].angle);
	graphicsView->setScene(&scene);
	graphicsView->show();
	graphicsView->scale(3,3);
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
	
}
bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
	timer.start(Period);
	return true;
}
void SpecificWorker::compute()
{
	histogram();
	ldata=laser_proxy->getLaserData();
	differentialrobot_proxy->getBaseState(Basestate);
	inner->updateTransformValues("base",Basestate.x,0,Basestate.z,0,Basestate.alpha,0);
	switch(st){
		case State::FINISH:
			break;
		case State::WORKING:
			gototarget();
			break;
		case State::IDLE:
			break;
		case State::BLOCKED:
			break;
	}
}
float SpecificWorker::go(const TargetPose &target)
{
	if(state.state=="WORKING"){
		QMutexLocker ml(&m);
		if(fabs(posetag.x-target.x)>200||fabs(posetag.z-target.z)>200){
			objetivoactual={target.x,target.y,target.z};
			stgo=statego::ORIENTARSE;
		}
		posetag={target.x,target.y,target.z};
	}
	else{
		state.state="WORKING";
		st=State::WORKING;
		QMutexLocker ml(&m);
		posetag={target.x,target.y,target.z};;
		objetivoactual={target.x,target.y,target.z};
	}
}
NavState SpecificWorker::getState()
{
	return state;
}
void SpecificWorker::stop()
{

}
void SpecificWorker::gototarget()
{
	writeinfo("poserobot:("+to_string(Basestate.x)+","+to_string(Basestate.z)+")\n"+"posetag:("+to_string(posetag.x)+","+to_string(posetag.z)+")\n"+"objetivoactual:("+to_string(objetivoactual.x)+","+to_string(objetivoactual.z)+")\n"+"subobjetivo:("+to_string(subobjetivo.x)+","+to_string(subobjetivo.z)+")");
	switch(stgo){
	  case statego::ORIENTARSE:
			 stgo=orientarse();
		break;
	  case statego::HAYOBTACULO:
			stgo=hayobtaculo();
		break;
	  case statego::PUEDOPASAR:
			stgo=puedopasar();
		break;
	  case statego::AVANZAR:
			stgo=avanzar();
		break;
	  case statego::CALCULAROBJETIVO:
			stgo=calcularsubobjetivo();
		break;
	  case statego::HELLEGADO:
			hellegado();
		break;
	}
}
SpecificWorker::statego SpecificWorker::orientarse()
{
	qDebug() << __FUNCTION__<<"---inicio";
	QVec objerobot=inner->transform("base",QVec::vec3(objetivoactual.x,0,objetivoactual.z),"world");
	float rot=atan2(objerobot(0),objerobot(2));
	if(fabs(rot)>muestreolaser){
		differentialrobot_proxy->setSpeedBase(0,rot*2);
		usleep(500000);
		differentialrobot_proxy->setSpeedBase(0,0);
	}
	qDebug() << __FUNCTION__<<"---fin";
	return statego::HAYOBTACULO;
}
SpecificWorker::statego SpecificWorker::hayobtaculo()
{
	qDebug() << __FUNCTION__<<"---inicio";
	QVec objerobot=inner->transform("laser",QVec::vec3(objetivoactual.x,0,objetivoactual.z),"world");
	float dist=objerobot.norm2();
	qDebug()<<ldata[50].dist<<"<"<<dist;
	qDebug() << __FUNCTION__<<"---fin";
	if(ldata[50].dist<dist)
		return statego::CALCULAROBJETIVO;
	else 
		return statego::PUEDOPASAR;
}
SpecificWorker::statego SpecificWorker::calcularsubobjetivo()
{
	qDebug() << __FUNCTION__<<"---inicio";
	int i,j;
	float disfinal;
	float anglefinal;
	for(i=51;i<100;i++){
		if((ldata[i].dist)-ldata[i-1].dist>100){
			break;
		}
	}
	for(j=49;j>=0;j--){
		if((ldata[j].dist)-ldata[i+1].dist>100){
			break;
		}
	}
	i--;
	j++;
		
	if(abs(i-50)<abs(50-j)){
		float dist1=ldata[i].dist;
		float dist2=230;
		float alpha=abs(asin(dist2/dist1));
		disfinal=ldata[i+2].dist/2;
		anglefinal=alpha+abs(ldata[i].angle);
		anglefinal=anglefinal;
	}
	else 
	{
		float dist1=ldata[j].dist;
		float dist2=230;
		float alpha=abs(asin(dist2/dist1));
		disfinal=ldata[j-2].dist/2;
		anglefinal=alpha+abs(ldata[j].angle);
		anglefinal=-anglefinal;
	}
	QVec obj=inner->laserTo("world","laser",disfinal,anglefinal);
	subobjetivo={obj(0),0,obj(2)};
	objetivoactual={obj(0),0,obj(2)};
	qDebug() << __FUNCTION__<<"---fin";
	return statego::ORIENTARSE;
}
SpecificWorker::statego SpecificWorker::puedopasar()
{	
	qDebug() << __FUNCTION__<<"---inicio";
	int i;
	QVec objerobot=inner->transform("base",QVec::vec3(objetivoactual.x,0,objetivoactual.z),"world");
	float distobje=sqrt(objerobot(0)*objerobot(0)+objerobot(2)*objerobot(2));
	for(i=51;i<100;i++)
	{
		float x=fabs(sin((ldata.data()+i)->angle)*(ldata.data()+i)->dist);
		float c=fabs(cos((ldata.data()+i)->angle)*(ldata.data()+i)->dist);
		if (x<210&&c<distobje)
		{
			return statego::CALCULAROBJETIVO;
		}
	}
	for(i=49;i>0;i--)
	{
		float x=fabs(sin((ldata.data()+i)->angle)*(ldata.data()+i)->dist);
		float c=fabs(cos((ldata.data()+i)->angle)*(ldata.data()+i)->dist);
		if (x<210&&c<distobje)
		{
			return statego::CALCULAROBJETIVO;
		}
	}
	return statego::AVANZAR;
}
void SpecificWorker::hellegado()
{
	qDebug() << __FUNCTION__<<"---inicio";
	if(fabs(Basestate.z-objetivoactual.z)<100&&fabs(Basestate.x-objetivoactual.x)<100)
	{
		m.lock();
		if (objetivoactual.x==posetag.x&&objetivoactual.z==posetag.z)
		{
			state.state="FINISH";
			st=State::FINISH;
		}
		else
			objetivoactual=posetag;
		m.unlock();
		differentialrobot_proxy->setSpeedBase(0,0);
		stgo=statego::ORIENTARSE;
	}
	else
	{
		QVec objerobot=inner->transform("base",QVec::vec3(objetivoactual.x,0,objetivoactual.z),"world");
		float rot=atan2(objerobot(0),objerobot(2));
		differentialrobot_proxy->setSpeedBase(500,rot);
	}
	qDebug() << __FUNCTION__<<"---fin";
}
SpecificWorker::statego SpecificWorker::avanzar()
{
	qDebug() << __FUNCTION__<<"---inicio";
	differentialrobot_proxy->setSpeedBase(600,0);
	qDebug() << __FUNCTION__<<"---fin";
	return statego::HELLEGADO;
}
void SpecificWorker::writeinfo(string _info)
{	
	texto->clear();
	QString *text=new QString(_info.c_str());
	texto->append(*text);
}  
void SpecificWorker::histogram()
{
	static QGraphicsPolygonItem *p;
	static QGraphicsLineItem *l, *sr, *sl, *safety;
	const float R = 500; //Robot radius
	const float SAFETY = 600;
	scene.removeItem(p);
	scene.removeItem(l);
	scene.removeItem(sr);
	scene.removeItem(sl);
	scene.removeItem(safety);
	
	//Search the first increasing step from the center to the right
	int i,j;
	for(i=(int)ldata.size()/2; i>1; i--)
	{
		if( (ldata[i].dist - ldata[i-1].dist) < -R )
		{
			int k=i-2;
			while( (k > 0) and (fabs( ldata[k].dist*sin(ldata[k].angle - ldata[i-1].angle)) < R ))
			{ k--; }
			i=k;
			break;
		}
	}
	for(j=(int)ldata.size()/2; j<(int)ldata.size()-2; j++)
	{
		if( (ldata[j].dist - ldata[j+1].dist) < -R )
		{
			int k=j+2;
			while( (k < (int)ldata.size()-1) and (fabs( ldata[k].dist*sin(ldata[k].angle - ldata[j+1].angle)) < R ))
			{ k++; }
			j=k;
			break;
		}
	}
	
	safety = scene.addLine(QLine(QPoint(0,-SAFETY/100),QPoint(ldata.size(),-SAFETY/100)), QPen(QColor(Qt::yellow)));
	sr = scene.addLine(QLine(QPoint(i,0),QPoint(i,-40)), QPen(QColor(Qt::blue)));
	sl = scene.addLine(QLine(QPoint(j,0),QPoint(j,-40)), QPen(QColor(Qt::magenta)));
	
	//DRAW		
	QPolygonF poly;
	int x=0;
	poly << QPointF(0, 0);
	
	for(auto d : ldata)
		poly << QPointF(++x, -d.dist/100); // << QPointF(x+5, d.dist) << QPointF(x+5, 0);
	poly << QPointF(x, 0);

	l = scene.addLine(QLine(QPoint(ldata.size()/2,0),QPoint(ldata.size()/2,-20)), QPen(QColor(Qt::red)));
  p = scene.addPolygon(poly, QPen(QColor(Qt::green)));
	
	scene.update();
	
	//select the best subtarget and return coordinates
}