/*!
*  Copyright (c) 2015 by hgaolbb
* \file layer.cpp
* \brief all layers implement
*/

#include "../include/layer.hpp"

namespace mini_net {

/*!
* \brief forward
*             X:        [N, C, Hx, Wx]
*             weight:   [F, C, Hw, Ww]
*             bias:     [F, 1, 1, 1]
*             out:      [N, F, 1, 1]
* \param[in]  const vector<Blob*>& in       in[0]:X, in[1]:weights, in[2]:bias
* \param[in]  const Param* param            params
* \param[out] Blob& out                     Y
*/
void AffineLayer::forward(const vector<Blob*>& in, Blob** out) {
    if (*out) {
        delete *out;
        *out = NULL;
    }
    int N = in[0]->get_N();
    int F = in[1]->get_N();
    
    *out = new Blob(N, F, 1, 1);
    //(*out)[0].print();
    for (int i = 0; i < N; ++i) {
        for (int f = 0; f < F; ++f) {
            //(*in[0])[0].print();
            //(*in[1])[0].print();
            //cout << accu((*in[0])[i] % (*in[1])[f]);// + (*in[2])[f];
            (**out)[i](0,0,f) = accu((*in[0])[i] % (*in[1])[f]) + (*in[2])[f](0,0,0);
            //cout << (**out)[i](0, 0, f) << endl;
            //(*out)[i].print("out[i] = \n");
        }
        //(*out)[i].print("###");
    }

    return;
}

/*!
* \brief backward
*             in:       [N, C, Hx, Wx]
*             weight:   [F, C, Hw, Ww]
*             bias:     [F, 1, 1, 1]
*             dout:     [N, F, 1, 1]
* \param[in]  const Blob* dout              dout
* \param[in]  const vector<Blob*>& cache    cache[0]:X, cache[1]:weights, cache[2]:bias
* \param[out] vector<Blob*>& grads          grads[0]:dX, grads[1]:dW, grads[2]:db
*/
void AffineLayer::backward(Blob* dout, const vector<Blob*>& cache, vector<Blob*>& grads) {
    Blob *dX = NULL;
    Blob *dW = NULL;
    Blob *db = NULL;
    
    int n = dout->get_N();

    Blob *pX = cache[0];
    auto szX = pX->size();
    Blob *pW = cache[1];
    auto szW = pW->size();
    Blob *pb = cache[2];
    auto szb = pb->size();

    // calc grads
    // dX
    mat mat_dx = (*dout).reshape() * (*pW).reshape();
    mat2Blob(mat_dx, &dX, (*pX).size());
    grads.push_back(dX);
    // dW
    mat mat_dw = (*dout).reshape().t() * (*pX).reshape();
    mat2Blob(mat_dw, &dW, (*pW).size());
    grads.push_back(dW);
    // db
    mat mat_db = (*dout).reshape().t() * mat(n, 1);
    mat_db.print();
    mat2Blob(mat_db, &db, (*pb).size());
    grads.push_back(db);

    return;
}

 //TODO

} //namespace mini_net
