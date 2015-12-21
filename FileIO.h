/* 
 * File:   FileIO.h
 * Author: Alex
 *
 * Created on September 28, 2015, 11:48 AM
 */

#ifndef FILEIO_H
#define	FILEIO_H

struct parameters{
    int lambda;
    int mu;
    int M;
    int numberOfArrivalsToSimulate;
};

void getParameters(struct parameters *);


#endif	/* FILEIO_H */

