TLegend theLegend (0.3, 0.6, 0.6, 0.95) ;
theLegend.AddEntry (h_0_NOH_NP_cosThetaEl, "noH, NP", "P") ;
theLegend.AddEntry (h_0_NOH_L_cosThetaEl , "noH, L",  "P") ;
theLegend.AddEntry (h_0_NOH_R_cosThetaEl , "noH, R",  "P") ;
theLegend.AddEntry (h_0_NOH_0_cosThetaEl , "noH, 0",  "P") ;
theLegend.AddEntry (h_0_NOH_NP_cosThetaEl, "noH, NP", "L") ;
theLegend.AddEntry (h_0_NOH_L_cosThetaEl , "noH, L",  "L") ;
theLegend.AddEntry (h_0_NOH_R_cosThetaEl , "noH, R",  "L") ;
theLegend.AddEntry (h_0_NOH_0_cosThetaEl , "noH, 0",  "L") ;

h_0_NOH_0_cosThetaEl->SetLineColor (kBlue) ;
h_0_NOH_L_cosThetaEl->SetLineColor (kGreen+2) ;
h_0_NOH_R_cosThetaEl->SetLineColor (kRed) ;
h_0_NOH_NP_cosThetaEl->SetLineColor (kGray+2) ;

h_0_SM_0_cosThetaEl->SetLineColor (kBlue) ;
h_0_SM_L_cosThetaEl->SetLineColor (kGreen+2) ;
h_0_SM_R_cosThetaEl->SetLineColor (kRed) ;
h_0_SM_NP_cosThetaEl->SetLineColor (kGray+2) ;

h_0_SM_0_cosThetaEl->SetLineWidth (2) ;
h_0_SM_L_cosThetaEl->SetLineWidth (2) ;
h_0_SM_R_cosThetaEl->SetLineWidth (2) ;
h_0_SM_NP_cosThetaEl->SetLineWidth (2) ;

TCanvas c0 ;
c0.cd () ;

h_0_SM_L_cosThetaEl->DrawClone ("histo") ;
h_0_SM_0_cosThetaEl->DrawClone ("histo same") ;
h_0_SM_NP_cosThetaEl->DrawClone ("histo same") ;
h_0_SM_R_cosThetaEl->DrawClone ("histo same") ;

h_0_NOH_L_cosThetaEl->DrawClone ("same") ;
h_0_NOH_0_cosThetaEl->DrawClone ("same") ;
h_0_NOH_NP_cosThetaEl->DrawClone ("same") ;
h_0_NOH_R_cosThetaEl->DrawClone ("same") ;

theLegend.Draw () ;

TCanvas c1 ;
c1.Divide (2,2) ;
c1.cd (1) ;
h_0_SM_NP_cosThetaEl->DrawClone ("histo") ;
h_0_NOH_NP_cosThetaEl->DrawClone ("same") ;
c1.cd (2) ;
h_0_NOH_0_cosThetaEl->DrawClone ("") ;
h_0_SM_0_cosThetaEl->DrawClone ("histo same") ;
c1.cd (3) ;
h_0_SM_L_cosThetaEl->DrawClone ("histo") ;
h_0_NOH_L_cosThetaEl->DrawClone ("same") ;
c1.cd (4) ;
h_0_SM_R_cosThetaEl->DrawClone ("histo") ;
h_0_NOH_R_cosThetaEl->DrawClone ("same") ;

TCanvas c2 ;
c2.Divide (2,2) ;
c2.cd (1) ;
h_0_SM_NP_cosThetaEl->DrawNormalized ("histo") ;
h_0_NOH_NP_cosThetaEl->DrawNormalized ("same") ;
c2.cd (2) ;
h_0_SM_0_cosThetaEl->DrawNormalized ("histo") ;
h_0_NOH_0_cosThetaEl->DrawNormalized ("same") ;
c2.cd (3) ;
h_0_SM_L_cosThetaEl->DrawNormalized ("histo") ;
h_0_NOH_L_cosThetaEl->DrawNormalized ("same") ;
c2.cd (4) ;
h_0_SM_R_cosThetaEl->DrawNormalized ("histo") ;
h_0_NOH_R_cosThetaEl->DrawNormalized ("same") ;

TCanvas c3 ;
c3.cd () ;

h_0_SM_L_cosThetaEl->DrawNormalized ("histo") ;
h_0_SM_0_cosThetaEl->DrawNormalized ("histo same") ;
h_0_SM_NP_cosThetaEl->DrawNormalized ("histo same") ;
h_0_SM_R_cosThetaEl->DrawNormalized ("histo same") ;

h_0_NOH_L_cosThetaEl->DrawNormalized ("same") ;
h_0_NOH_0_cosThetaEl->DrawNormalized ("same") ;
h_0_NOH_NP_cosThetaEl->DrawNormalized ("same") ;
h_0_NOH_R_cosThetaEl->DrawNormalized ("same") ;

theLegend.Draw () ;


