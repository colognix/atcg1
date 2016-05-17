//=============================================================================
//
//       Copyright (C) 2010 by Computer Graphics Group, University of Bonn
//                           cg.cs.uni-bonn.de
//
//-----------------------------------------------------------------------------
//
//	part of RMI1, SS10, Excercise 2
//
//

//=============================================================================
//
//  CLASS CGaussShader - IMPLEMENTATION
//
//=============================================================================



//== INCLUDES =================================================================
#include <cmath>
#include "GaussShader.h"

//== NAMESPACE ===============================================================

namespace RAYTRACER {

//== IMPLEMENTATION ==========================================================


#define INV_SQRT_2_PI 0.39894228040143267793994605993438

template<typename ScalarT>
ScalarT sqr(ScalarT x)
{
	return x*x;
}


template<typename ScalarT>
ScalarT Gauss(ScalarT mean, ScalarT sigma, ScalarT x)
{
	ScalarT inv_sigma = 1.0f/sigma;
	return inv_sigma*ScalarT(INV_SQRT_2_PI) * std::exp(ScalarT(-0.5)*sqr((x-mean)*inv_sigma));
}

ColorType
CGaussShader::Shade(const TTracingContext& tContext) const
{
	/*

		Implement:

		A simple BRDF model using a Gauss lobe centered around the reflection direction.
		The final color consists of a perfectly diffuse part (Lambert) and a highlight part (Gauss)

		Relevant fields in TTracingContext are:
		v3Outgoing        - outgoing direction
		v3Incoming        - incoming direction
		v3Normal          - normal vector of surface
		colLightIntensity - color of incident light from v3Incoming

		The relevant member variables are:
		m_colDiffuse	- color of diffuse part
		m_colHighLight	- color of highlight part
		m_fSigma		- variance of gauss lobe

	*/
	
	ColorType result(0);
	const VectorType3& v3_normal = tContext.v3Normal;
	RealType r_cos_theta = v3_normal | tContext.v3Incoming;
	
	
	if (r_cos_theta > 0)
	{
		VectorType3 v3_ideal = -tContext.v3Incoming + 2. *  r_cos_theta * v3_normal;
		const ColorType& colLightIntensity = tContext.colLightIntensity;
		// add lambertian term
		result += colLightIntensity*m_colDiffuse*r_cos_theta;
		// add gaussian term
		result += colLightIntensity*m_colHighLight*Gauss(0., m_fSigma, 1. - (tContext.v3Outgoing | v3_ideal));
	}
	

	return result;
};

//=============================================================================
}; // namespace RAYTRACER
//=============================================================================


