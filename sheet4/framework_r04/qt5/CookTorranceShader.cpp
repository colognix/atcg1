//=============================================================================
//
//       Copyright (C) 2010 by Computer Graphics Group, University of Bonn
//                           cg.cs.uni-bonn.de
//
//-----------------------------------------------------------------------------
//
//	part of RMI1, SS10, Excercise 3
//
//=============================================================================

//=============================================================================
//
//  CLASS CCookTorranceShader - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================
#define _USE_MATH_DEFINES
#include "CookTorranceShader.h"
#include <math.h>
#include <algorithm>


//== NAMESPACE ===============================================================

namespace RAYTRACER {


//== IMPLEMENTATION ==========================================================




ColorType
CCookTorranceShader::Shade(const TTracingContext& tContext) const
{
	ColorType result(0);

	const VectorType3& v3_L = tContext.v3Incoming;
	const VectorType3& v3_V = tContext.v3Outgoing;
	const VectorType3& v3_N = tContext.v3Normal;

	VectorType3 v3_H = v3_L + v3_V;
	v3_H.normalize();

	RealType r_scp_NH = v3_N | v3_H;
	RealType r_alpha = acos(r_scp_NH);
	RealType r_costheta = v3_V | v3_N;
	RealType r_scp_VH = v3_V | v3_H;	// V | H = L | H by contruction of H
	RealType r_scp_NL = v3_N | v3_L;
	RealType r_scp_LH = v3_H | v3_L;

	if (r_scp_NH == 0 || r_costheta <= 0 || r_scp_NL == 0 || r_scp_LH == 0 || r_scp_NH == 0)
		return result;

	RealType r_D = exp(-pow(tan(r_alpha) / m_m, 2)) / pow(m_m * r_scp_NH * r_scp_NH, 2);

	RealType r_G = std::min(r_scp_NL/r_scp_LH, r_costheta/r_scp_VH);
	r_G = std::min(2.*r_G*r_scp_NH , 1.);
	
	RealType r_n = (1. + sqrt(m_F0)) / (1. - sqrt(m_F0));
	RealType r_g = sqrt(r_n*r_n + r_scp_VH*r_scp_VH - 1);
	RealType r_gcSum = r_g + r_scp_VH;
	RealType r_gcDiff = r_g - r_scp_VH;
	RealType r_F = 0.5 * r_gcDiff * r_gcDiff / (r_gcSum * r_gcSum)*(1 + pow((r_scp_VH * r_gcSum - 1.) / (r_scp_VH * r_gcDiff + 1.), 2));

	//if (r_F < m_F0)
	//	return result;

	RealType r_CT = 1./M_PI * r_D * r_G * r_F / (r_scp_NL * r_costheta);

	
	
	
	if (r_F > m_F0)
	{
		RealType c_colorIp = (r_F - m_F0) / (1. - m_F0) ;
		result.SetRgb(m_colHighLight.red()+(tContext.colLightIntensity.red() - m_colHighLight.red())*c_colorIp,
			m_colHighLight.green() + (tContext.colLightIntensity.green() - m_colHighLight.green())*c_colorIp,
			m_colHighLight.blue() + (tContext.colLightIntensity.blue() - m_colHighLight.blue())*c_colorIp);
		result *= r_CT;
		result += (m_colDiffuse+m_colAmbient)*0.5 * r_G;
	}
	else
		result = m_colHighLight*r_CT + m_colAmbient * r_G;
	result *= tContext.colLightIntensity*r_costheta;
	//result += tContext.colLightIntensity*m_colDiffuse*r_costheta;

	return result;
};

//=============================================================================
}; // namespace RAYTRACER
//=============================================================================


