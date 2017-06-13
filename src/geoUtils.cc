#include "geoUtils.h"

using namespace Legendre ;

void invertXY (TLorentzVector & p){ //rotates p into the frame with W along Z axis
    double x_temp = p.Px();
    double y_temp = p.Py();
    p.SetPx(-x_temp);
    p.SetPy(-y_temp);
    return;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


double coeff_legendre (TH1F* theta, unsigned int a){ // the histogram must be normalised s.t. \int_{-1}^{+1} dx F (x) = 1 
            double result = 0.;
            double F = 0.;
            double x = 0.;
            double pl = 0.;
            double dx = (theta->GetXaxis()->GetXmax() - theta->GetXaxis()->GetXmin()) / theta->GetNbinsX();           
            for(int j = 1; j <= theta->GetNbinsX(); j++){                      
        F = theta->GetBinContent(j);
        x = theta->GetXaxis()->GetBinCenter(j);  
        pl = Pn<double>(a,x);                
                        result += (2*a+1)*dx*F*0.5*pl;
            }                                
    return result;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


double coeff_legendre_new (TH1F* theta, unsigned int a){ // the histogram  must not be normalised
            double result = 0.;
            double F = 0.;
            double x = 0.;
            double pl = 0.;
            double N = 0;          
             for(int j = 1; j <= theta->GetNbinsX(); j++){                      
                N += theta->GetBinContent(j);
            }                                
            for(int j = 1; j <= theta->GetNbinsX(); j++){                      
                F = theta->GetBinContent(j);
                x = theta->GetXaxis()->GetBinCenter(j);  
                pl = Pn<double>(a,x);                
                result += (2*a+1)*F*0.5*pl / N;
            }                                
    return result;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


double coeff_legendre_ERR(TH1F* theta, unsigned int a){ // the histogram  must not be normalised
            double result = 0.;
            double temp = 0.;
            double F = 0.;
            double pl = 0.;
            double x = 0.;
            double N = 0;                     
            for(int j = 1; j <= theta->GetNbinsX(); j++){                      
                N += theta->GetBinContent(j);
            }                                                 
            for(int j = 1; j <= theta->GetNbinsX(); j++){                      
                F = theta->GetBinContent(j);
                x = theta->GetXaxis()->GetBinCenter(j);  
                pl = Pn<double>(a,x);                
                temp += F*pl;
            } 
            for(int j = 1; j <= theta->GetNbinsX(); j++){    
                x = theta->GetXaxis()->GetBinCenter(j);  
                pl = Pn<double>(a,x);                    
                double dalphai = (pl / N) - (temp / pow(N,2.));
                result += pow(dalphai,2.) * theta->GetBinContent(j);
            }                                            
    return (2*a+1.)*0.5*pow(result, 0.5);
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


double integral (TH1F* theta){
            double result = 0.;
            double F = 0.;
            double dx = (theta->GetXaxis()->GetXmax() - theta->GetXaxis()->GetXmin()) / theta->GetNbinsX();           
            for(int j = 1; j <= theta->GetNbinsX(); j++){                      
            F = theta->GetBinContent(j);               
            result += dx*F;
            }                                
    return result;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


double L2norm (TH1F* theta){
            double result = 0.;
            double F = 0.;
            double x = 0.;
            double dx = (theta->GetXaxis()->GetXmax() - theta->GetXaxis()->GetXmin()) / theta->GetNbinsX();           
            for(int j = 1; j <= theta->GetNbinsX(); j++){                      
                F = theta->GetBinContent(j);
                x = theta->GetXaxis()->GetBinCenter(j);                  
                result += dx*F*F;
            }                                
    return result;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


double fform (double x, double f0, double fL, double fR){
    double p0 = (3./4.)*(1.-x*x);
    double pL = (3./8.)*(1.-x)*(1.-x);
    double pR = (3./8.)*(1.+x)*(1.+x);
    return p0*f0 + pL*fL + pR*fR;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void fillexp (TH1F* theta, double f0, double fL, double fR){
    double f = 0.;
    for(int j = 1; j <= theta->GetNbinsX(); j++){                  
        f = fform(theta->GetXaxis()->GetBinCenter(j),f0,fL,fR);
        theta->SetBinContent(j,f);                                             
    }                 
    return;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


double deltaPhi (double phi1, double phi2){
    double deltaphi=fabs(phi1-phi2);
    if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
    if (deltaphi > 3.141592654) deltaphi = 6.283185308-deltaphi;
    return deltaphi;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void rotIntoW (TLorentzVector & p, TLorentzVector W){ //rotates p into the frame with W along Z axis
    p.RotateZ(-W.Phi());
    p.RotateY(-W.Theta()); 
    return;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void boostIntoW (TLorentzVector & p, TLorentzVector W){
    double beta = W.Beta();
    double pp = pow(W.Px()*W.Px() +W.Py()*W.Py()+W.Pz()*W.Pz(), 0.5);
    TVector3 ww (beta*W.Px()/pp, beta*W.Py()/pp, beta*W.Pz()/pp);       
    p.Boost(-ww.Px(),-ww.Py(),-ww.Pz());
    return;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void printTLV (TString aa , TLorentzVector ff){
    std::cout << aa << " =\t( " << ff.E() << "  \t,  " << ff.Px() << "  \t,  " << ff.Py() << "  \t,  " << ff.Pz() <<")\n";
    return;
}

