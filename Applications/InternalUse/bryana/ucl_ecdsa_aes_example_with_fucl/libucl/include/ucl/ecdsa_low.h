void ecc_mult_jacobian_window4_p160_precompQ(DIGIT *xres,DIGIT *yres, DIGIT *m, DIGIT *a,DIGIT *x,DIGIT *y,DIGIT *p, DIGIT qx[40][8],DIGIT qy[40][8],DIGIT digits,int curve);
void ecc_mult_jacobian_window4_p192_precompQ(DIGIT *xres,DIGIT *yres, DIGIT *m, DIGIT *a,DIGIT *x,DIGIT *y,DIGIT *p, DIGIT qx[48][16],DIGIT qy[48][16],DIGIT digits,int curve);
void __API__ ecc_mult_jacobian_window4_p256_precompQ(DIGIT *xres,DIGIT *yres, DIGIT *m, DIGIT *a,DIGIT *x,DIGIT *y,DIGIT *p, DIGIT qx[64][8],DIGIT qy[64][8],DIGIT digits,int curve);
void __API__ ecc_mult_jacobian_window4_bp256_precompQ(DIGIT *xres,DIGIT *yres, DIGIT *m, DIGIT *a,DIGIT *x,DIGIT *y,DIGIT *p, DIGIT qx[64][8],DIGIT qy[64][8],unsigned int digits,int curve);
void ecc_mult_jacobian_window4_p384_precompQ(DIGIT *xres,DIGIT *yres, DIGIT *m, DIGIT *a,DIGIT *x,DIGIT *y,DIGIT *p, DIGIT qx[96][8],DIGIT qy[96][8],DIGIT digits,int curve);
void ecc_mult_jacobian_window4_p224_precompQ(DIGIT *xres,DIGIT *yres, DIGIT *m, DIGIT *a,DIGIT *x,DIGIT *y,DIGIT *p, DIGIT qx[56][8],DIGIT qy[56][8],DIGIT digits,int curve);
int __API__ ecc_add(DIGIT *x3,DIGIT *y3,DIGIT *x1,DIGIT *y1,DIGIT *x2,DIGIT *y2,DIGIT *p,DIGIT digits,int curve);
int __API__ ecc_double(DIGIT *x3,DIGIT *y3,DIGIT *x1,DIGIT *y1,DIGIT *a,DIGIT *p, DIGIT digits,int curve);
void precompute_inv(DIGIT *inv,DIGIT *p,int digits,int curve);
