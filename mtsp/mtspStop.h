/* 
 * File:   mtspStop.h
 * Author: tieuminh
 *
 * Created on April 8, 2015, 3:57 PM
 */

#ifndef MTSPSTOP_H
#define	MTSPSTOP_H

class mtspStop {
public:
    mtspStop();
    mtspStop(const mtspStop& orig);
    virtual ~mtspStop();
private:
    unsigned int _come;
    unsigned int _wait;
    unsigned int _late;
    unsigned int _id;
};

#endif	/* MTSPSTOP_H */

