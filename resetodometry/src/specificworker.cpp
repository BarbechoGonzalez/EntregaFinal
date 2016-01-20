/*
 *    Copyright (C) 2016 by YOUR NAME HERE
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
// 	inner=NULL;
// 	inner = new InnerModel("/home/ivan/robocomp/files/innermodel/RoCKIn@home/world/apartment.xml");
	inner = new InnerModel("/home/ivan/robocomp/files/innermodel/simpleworld.xml");
	avistado=false;
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
// 	try
// 	{
// 		RoboCompCommonBehavior::Parameter par = params.at("InnerModelPath");
// 		string innermodel_path=par.value;
// 		try
// 		{	
// 			inner = new InnerModel(innermodel_path);
// 		}
// 		catch (QString &s)
// 		{
// 			printf("error reading iner model %s\n", s.toStdString().c_str());
// 		}
// 		
// 	}
// 	catch(std::exception e) { qFatal("Error reading config params"); }
	
	return true;
}

void SpecificWorker::compute()
{
	if(avistado)
	{	
		Resetodometry();
		avistado=false;
	}
}

void SpecificWorker::Resetodometry()
{
	try{
		InnerModelNode *T = inner->getNode("rgbd");
		
		inner->InnerModel::newTransform("tag", "static", T, tag.tx, tag.ty, tag.tz, tag.rx, tag.ry, tag.rz);
		RTMat r=inner->getTransformationMatrix("tag","rgbd");
		QVec v=inner->transform6D("tag","rgbd");
		T = inner->getNode(tag.id+"");
		inner->newTransform("RGBDvirtual", "static",T , v.x(), v.y(), v.z(), v.rx(), v.ry(), v.rz());
		v=inner->transform6D("rgbd","robot");
		T = inner->getNode("RGBDvirtual");
		inner->newTransform("basevirtual", "static",T ,v.x(),v.y(),v.z(),v.rx(),v.ry(),v.rz());
		v=inner->transform6D("basevirtual","world");
		inner->updateTransformValues("robot",v.x(),v.y(),v.z(),v.rx(),v.ry(),v.rz());

		TBaseState t;
		t.x=v.x();
		t.z=v.z();
		t.alpha=v.ry();
		differentialrobot_proxy->setOdometer(t);
		
		inner->removeNode("basevirtual");
		inner->removeNode("RGBDvirtual");
		inner->removeNode("tag");
		qDebug() << __FUNCTION__<<"Odometry: ";
		qDebug() <<"	tx:"<<v.x();
		qDebug() <<"	ty:"<<v.y();
		qDebug() <<"	tz:"<<v.z();
		qDebug() <<"	rx:"<<v.rx();
		qDebug() <<"	ry:"<<v.ry();
		qDebug() <<"	rz:"<<v.rz();
	}
	catch(const Ice::Exception &ex)
	{
		  std::cout << ex << std::endl;
	}
}

void SpecificWorker::newAprilTag(const tagsList &tags)
{
	if(!avistado)
		for(auto t:tags){
			tag=t;
		}
		avistado=true;
}







