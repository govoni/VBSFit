#ifndef hFunctions_h
#define hFunctions_h

#include <functional>
#include "hChain.h"
#include <vector>

struct setH1Colors : public std::unary_function<hChain *, void>
{
  setH1Colors (std::vector<int> colors) :
    m_colors (colors)
    {}
  
  void operator() (hChain * arg) 
    {
      arg->SetColors (m_colors) ;
    }

  private :
    
    std::vector<int> m_colors ;
  
} ;
                

//PG --------------------------------------------------------   


/*
struct setH2Colors : public std::unary_function<h2Chain *, void>
{
  setH2Colors (std::vector<int> colors) :
    m_colors (colors)
    {}
  
  void operator() (h2Chain * arg) 
    {
      arg->SetColors (m_colors) ;
    }

  private :
    
    std::vector<int> m_colors ;
  
} ;
*/

               
//PG --------------------------------------------------------   


struct scaleH1 : public std::unary_function<hChain *, void>
{
  scaleH1 (int index, double scale) :
    m_index (index) ,
    m_scale (scale)
    {}
  
  void operator() (hChain * arg) 
    {
      arg->Scale (m_index, m_scale) ;
    }

  private :
    
    int m_index ;
    double m_scale ;
  
} ;

               
//PG --------------------------------------------------------   


/*
struct scaleH2 : public std::unary_function<h2Chain *, void>
{
  scaleH2 (int index, double scale) :
    m_index (index) ,
    m_scale (scale)
    {}
  
  void operator() (h2Chain * arg) 
    {
      arg->Scale (m_index, m_scale) ;
    }

  private :
    
    int m_index ;
    double m_scale ;
  
} ;
*/

             
//PG --------------------------------------------------------   


struct printEachH1 : public std::unary_function<hChain *, void>
{
  printEachH1 (bool isLog = false, int rebin = 1, TString name = "default") :
    m_isLog (isLog) ,
    m_rebin (rebin) , 
    m_name  (name)
    {}
  void operator() (hChain * arg) 
    {
      arg->PrintEach (m_isLog, m_rebin, m_name) ;
    }

  private :
  
    bool m_isLog ;
    int m_rebin ;
    TString m_name ;
    
} ;


//PG --------------------------------------------------------   


/*
struct printEachH2 : public std::unary_function<h2Chain *, void>
{
  void operator() (h2Chain * arg) 
    {
      arg->PrintEach () ;
    }

  private :
    
} ;
*/


#endif
