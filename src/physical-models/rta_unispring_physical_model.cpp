/**
 * @file rta_unispring_physical_model.cpp
 * @author Norbert Schnell
 *
 * @copyright
 * Copyright (C) 2008 - 2009 by IRCAM-Centre Georges Pompidou, Paris, France.
 * All rights reserved.
 * 
 * License (BSD 3-clause)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "rta_unispring.h"
#define DIM RTA_UNISPRING_NDIM

using namespace UniSpringSpace ;


/** 
 Compute euclidean distance between points i1 and i2.
 */
double UniSpring::euclDistance(int i1, int i2){
	
	double diffx = mPoints[i1].x() - mPoints[i2].x();
	double diffy = mPoints[i1].y() - mPoints[i2].y();
	double dist = pow(diffx,2) + pow(diffy,2);
	
    return sqrt(dist);
	
}

double UniSpring::euclDistance_3D(int i1, int i2){
	
	double diffx = mPoints[i1].x() - mPoints[i2].x();
	double diffy = mPoints[i1].y() - mPoints[i2].y();
	double diffz = mPoints[i1].z() - mPoints[i2].z();
	double dist = pow(diffx,2) + pow(diffy,2) + pow(diffz,2);
	
    return sqrt(dist);
	
}


/** 
 Compute displacement of point i1 from its previous position.
 */
double UniSpring::euclDispl(int i1){
	
	double diffx = mPoints[i1].x() - mPointsOld[i1].x();
	double diffy = mPoints[i1].y() - mPointsOld[i1].y();
	double dist = pow(diffx,2) + pow(diffy,2);
	
    return sqrt(dist);
	
}

double UniSpring::euclDispl_3D(int i1){
	
	double diffx = mPoints[i1].x() - mPointsOld[i1].x();
	double diffy = mPoints[i1].y() - mPointsOld[i1].y();
	double diffz = mPoints[i1].z() - mPointsOld[i1].z();
	double dist = pow(diffx,2) + pow(diffy,2) + pow(diffz,2);
	
    return sqrt(dist);
	
}


/** 
 Sum all elements of a vector.
 */
double UniSpring::sum(std::vector<double> v){
	
	double sum = 0;
	
	for(std::vector<double>::iterator it = v.begin(); it!=v.end(); it++){
		
		sum += *it;
	}
	
	return sum;
	
}

						
/** 
 Definition of the desired length function.
 */
double UniSpring::fh(double px, double py){
	
	double desdist = 1; // Uniform
	return desdist;
	
}

double UniSpring::fh_3D(double px, double py, double pz){
	
	double desdist = 1; // Uniform
	return desdist;
	
}

/** 
 Update point positions.
 */
void UniSpring::updatePositions(){
	
	double deps = sqrt(EPS)*H0;
	
	// Compute edge lengths and initial target distances
	for (int i = 0; i < mEdges.size(); i++) {
		
		double middlex;
		double middley;
		double length;
		
		middlex = (mPoints[mEdges[i][0]].x()+mPoints[mEdges[i][1]].x())/2;
		middley = (mPoints[mEdges[i][0]].y()+mPoints[mEdges[i][1]].y())/2;
		length = euclDistance(mEdges[i][0],mEdges[i][1]);
				
		hbars2.push_back(pow(fh(middlex,middley),2)); // compute squared value of target distance on edge's middle
		L.push_back(length);
		L2.push_back(pow(length,2));
		
	}
	
	// Compute total edge length and initial target distance	
	hbars2_sum = sum(hbars2);
	L2_sum = sum(L2);
	
	// Compute total force components
	for (int i = 0; i < mEdges.size(); i++) {
		
		std::vector<double> Fvec(2);
		std::vector<double> barvec(2);
		
		// Compute edge force
		double L0 = sqrt(hbars2[i]) * FSCALE * sqrt(L2_sum/hbars2_sum);
		double F = std::max(L0-L[i],0.);
		
		// Get edge unit vector
		barvec[0] = ( mPoints[mEdges[i][0]].x() - mPoints[mEdges[i][1]].x() ) / L[i];
		barvec[1] = ( mPoints[mEdges[i][0]].y() - mPoints[mEdges[i][1]].y() ) / L[i];		
		
		// Project force on edge unit vector
		Fvec[0] = F * barvec[0];
		Fvec[1] = F * barvec[1];
		
		// Assign force due to current edge to point indices in Ftot		
		//int currentIndex = mEdges[i][0]; //debug
		
		Ftot[mEdges[i][0]][0] += Fvec[0];
		Ftot[mEdges[i][0]][1] += Fvec[1];
		Ftot[mEdges[i][1]][0] += -Fvec[0];
		Ftot[mEdges[i][1]][1] += -Fvec[1];
		
	}
	
	// Move points & bring outside points back to boundary
	std::vector<double> displ_temp(2);
	
	for (int i=0; i<mNpoints; i++) {
		
		displ_temp[0] = DELTAT * Ftot[i][0];
		displ_temp[1] = DELTAT * Ftot[i][1];
		
		mPoints[i].setPosition( mPoints[i].x() + displ_temp[0], mPoints[i].y() + displ_temp[1]);	
		
		// Check if point has moved outside
		double d = mShape->fd_compute(mPoints[i].x(), mPoints[i].y());
		
		if (d > 0) {
			
			//Bring it back to boundary
			double dgradx = ( mShape->fd_compute(mPoints[i].x() + deps, mPoints[i].y()) - d ) / deps;
			double dgrady = ( mShape->fd_compute(mPoints[i].x(), mPoints[i].y() + deps) - d ) / deps;
			
			mPoints[i].setPosition( mPoints[i].x() - d * dgradx, mPoints[i].y() - d * dgrady);
			
			
		}
				
		// Compute displacement relative to previous triangulation (_old) and previous step (_prev)
		double displ_old = euclDispl(i);
		double displ_prev = sqrt(pow(displ_temp[0],2) + pow(displ_temp[1],2));
		
		// For interior point, update max displacement relative to previous step
		if (d < -GEPS && displ_prev > max_displ_prev) {
			
			max_displ_prev = displ_prev;
			
		}
		
		// For all points, update max displacement relative to previous triangulation
		if (displ_old > max_displ_old) {
			
			max_displ_old = displ_old;
			
		}
		
	}
	
}

void UniSpring::updatePositions_3D(){
	
	double deps = sqrt(EPS)*H0;
	
	// Compute edge lengths and initial target distances
	for (int i = 0; i < mEdges.size(); i++) {
		
		double middlex;
		double middley;
		double middlez;
		double length;
		
		middlex = (mPoints[mEdges[i][0]].x()+mPoints[mEdges[i][1]].x())/2;
		middley = (mPoints[mEdges[i][0]].y()+mPoints[mEdges[i][1]].y())/2;
		middlez = (mPoints[mEdges[i][0]].z()+mPoints[mEdges[i][1]].z())/2;
		length = euclDistance_3D(mEdges[i][0],mEdges[i][1]);
		
		//double length3 = pow(length,3);
		
		hbars3.push_back(pow(fh_3D(middlex,middley,middlez),3)); // compute squared value of target distance on edge's middle
		L.push_back(length);
		L3.push_back(pow(length,3));
		
	}
	
	// Compute total edge length and initial target distance	
	hbars3_sum = sum(hbars3);
	L3_sum = sum(L3);
	
	// Compute total force components
	for (int i = 0; i < mEdges.size(); i++) {
		
		std::vector<double> Fvec(3);
		std::vector<double> barvec(3);
		
		// Compute edge force
		double L0 = pow(hbars3[i],(double)1/3) * FSCALE * pow(L3_sum/hbars3_sum,(double)1/3);
		double F = std::max(L0-L[i],0.);
		
		// Get edge unit vector
		barvec[0] = ( mPoints[mEdges[i][0]].x() - mPoints[mEdges[i][1]].x() ) / L[i];
		barvec[1] = ( mPoints[mEdges[i][0]].y() - mPoints[mEdges[i][1]].y() ) / L[i];	
		barvec[2] = ( mPoints[mEdges[i][0]].z() - mPoints[mEdges[i][1]].z() ) / L[i];
		
		// Project force on edge unit vector
		Fvec[0] = F * barvec[0];
		Fvec[1] = F * barvec[1];
		Fvec[2] = F * barvec[2];
		
		// Assign force due to current edge to point indices in Ftot		
		
		Ftot[mEdges[i][0]][0] += Fvec[0];
		Ftot[mEdges[i][0]][1] += Fvec[1];
		Ftot[mEdges[i][0]][2] += Fvec[2];
		Ftot[mEdges[i][1]][0] += -Fvec[0];
		Ftot[mEdges[i][1]][1] += -Fvec[1];
		Ftot[mEdges[i][1]][2] += -Fvec[2];
		
	}
	
	// Move points & bring outside points back to boundary
	std::vector<double> displ_temp(3);
	
	for (int i=0; i<mNpoints; i++) {
		
		displ_temp[0] = DELTAT * Ftot[i][0];
		displ_temp[1] = DELTAT * Ftot[i][1];
		displ_temp[2] = DELTAT * Ftot[i][2];
		
		mPoints[i].setPosition(mPoints[i].x() + displ_temp[0], mPoints[i].y() + displ_temp[1], mPoints[i].z() + displ_temp[2]);
		
		// Check if point has moved outside
		double d = mShape_3D->fd_compute(mPoints[i].x(), mPoints[i].y(), mPoints[i].z());
		
		if (d > 0) {
			
			//Bring it back to boundary
			double dgradx = ( mShape_3D->fd_compute(mPoints[i].x() + deps, mPoints[i].y(), mPoints[i].z()) - d ) / deps;
			double dgrady = ( mShape_3D->fd_compute(mPoints[i].x(), mPoints[i].y() + deps, mPoints[i].z()) - d ) / deps;
			double dgradz = ( mShape_3D->fd_compute(mPoints[i].x(), mPoints[i].y(), mPoints[i].z() + deps) - d ) / deps;
			
			mPoints[i].setPosition(mPoints[i].x() - d * dgradx, mPoints[i].y() - d * dgrady, mPoints[i].z() - d * dgradz);
					
		}
		
		// Compute displacement relative to previous triangulation (_old) and previous step (_prev)
		double displ_old = euclDispl_3D(i);
		double displ_prev = sqrt(pow(displ_temp[0],2) + pow(displ_temp[1],2) + pow(displ_temp[2],2));
		
		// For interior point, update max displacement relative to previous step
		if (d < -GEPS && displ_prev > max_displ_prev) {
			
			max_displ_prev = displ_prev;
			
		}
		
		// For all points, update max displacement relative to previous triangulation
		if (displ_old > max_displ_old) {
			
			max_displ_old = displ_old;
			
		}
		
	}
	
}


/** 
 Prepare physical model for next iteration.
 */
void UniSpring::resetPhysicalModel(){
	
	max_displ_old = 0;
	max_displ_prev = 0;
	std::vector<double> F_temp(2,0); 
	for (int i=0; i<mNpoints; i++) {
		
		Ftot[i]=F_temp;
		
	}
	hbars2.clear();
	L.clear();
	L2.clear();
	
}

void UniSpring::resetPhysicalModel_3D(){
	
	max_displ_old = 0;
	max_displ_prev = 0;
	std::vector<double> F_temp(3,0); 
	for (int i=0; i<mNpoints; i++) {
		
		Ftot[i]=F_temp;
		
	}
	hbars3.clear();
	L.clear();
	L3.clear();
	
}