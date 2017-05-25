//=============================================================================
// Blur.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Performs a separable blur with a blur radius of 5.  
//=============================================================================


struct General_Infor
{
	float4 numTriangelX_numSamplesY;
	float4 PixelWidthX_HeightY;
};



struct SphereObj
{
	float4 CenterPos;
	float4 Radius;
	float4 emit;
	float4 reflect;
	float4 materialTypeX_SmoothY;
};


struct PlaneObj
{

	float4 Normal;
	float4 Point;
	float4 Color;
	float4 Reflection_X;

};

struct TriangelObj
{
	float4 posX;
	float4 posY;
	float4 posZ;
	float4 normal;
	float4 emit;
	float4 reflect;
	float4 materialTypeX_SmoothY;

};

struct RandValue
{
	float data[10];
};



StructuredBuffer<General_Infor> infor_Data: register(t0);

StructuredBuffer<SphereObj> SphereObj_Data: register(t1);

StructuredBuffer<TriangelObj> TriangelObj_Data: register(t3);

StructuredBuffer<RandValue> Rand_Data: register(t6);

Texture2D<float4> gInput: register(t5);
RWTexture2D<float4> gOutput: register(u0);



static float widthPixel = 1600.0;
static float heightPixel = 1200.0;

static float widthPiexlSize = 2.666 / widthPixel;
static float heightPiexlSize = 2.0 / heightPixel;


static float3 eyePos = float3(0, 0, 0);



float4  rayGetPoint(float3 ray, float3 original)
{

	float MinLenToView = 100000;
	float OBJindex = 0;
	float3 rayTracingPoint = float3(-100, -100, -100);
	ray = normalize(ray);
	float eps = 0.00001;

	///Sphere Test
	/*
	for (int i = 0; i < 9; i++)
	{

		float3 Op = SphereObj_Data[i].CenterXYZ_RadiusW.xyz - original;

		float  b = dot(Op, ray);
		float  det = b*b - dot(Op, Op) + SphereObj_Data[i].CenterXYZ_RadiusW.w *SphereObj_Data[i].CenterXYZ_RadiusW.w;

		if (det >= 0)
		{
			det = sqrt(det);
			float t = b - det;
			if (t > eps)
			{
				if (t <= MinLenToView)
				{
					MinLenToView = t;
					OBJindex = i;
					rayTracingPoint = original + ray * t;
				}
				else 
				{
					t = b + det;
					if (t > eps)
					if (t <= MinLenToView)
					{
					MinLenToView = t;
					OBJindex = i;
					rayTracingPoint = original + ray * t;
					}
				}
			}


		}
	}

	struct TriangelObj
	{
	float4 posX;
	float4 posY;
	float4 posZ;
	float4 normal;
	float4 emit;
	float4 reflect;
	float4 materialTypeX_SmoothY;
	};
	*/
	for (int i = 0; i < ((int)infor_Data[0].numTriangelX_numSamplesY.x); i++)
	{
		float4 a_b = TriangelObj_Data[i].posX - TriangelObj_Data[i].posY;
			float4 a_c = TriangelObj_Data[i].posX - TriangelObj_Data[i].posZ;
			float4 a_pos = TriangelObj_Data[i].posX - float4(original, 0);

			float3 a_bCross_a_pos = cross(a_b.xyz, a_pos.xyz);
			float3 a_cCross_dir = cross(a_c.xyz, ray);

			double m = 1.0 / dot(a_b.xyz, a_cCross_dir);
			
		double t = dot(a_c.xyz, a_bCross_a_pos)*(-m);
		
		if (t > 0)
		{
			
			double u = dot(a_pos.xyz,a_cCross_dir) * m;
			if (u >= 0.0 && u <= 1.0) 
			{

				double v = dot(ray,a_bCross_a_pos) * m;
				if (v >= 0.0 && v <= (1.0 - u))
				{
					if (t < MinLenToView)
					{
						OBJindex = i;
						MinLenToView = t;
						rayTracingPoint = original + ray * t;
					}
					
				}
			}
		}


	}


	return float4(rayTracingPoint, OBJindex);

}



float3 GetCross(float3 u, float3 v)
{
	return float3(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x);
}

float3 GetMul(float3 u, float x)
{
	
	return float3(u.x*x, u.y*x, u.z*x);

}



#define MaxTracingDepth 5
#define NO_POINT (Get.x == -100 && Get.y == -100 && Get.z == -100)
#define MAT_Reflect  0
#define MAT_Diffuse  1


float4 PathTracing(float3 Ray, float3 Original, int2 offset)
{

	float4 Color[5];
	Color[0] = float4(0,0,0,0);
	int AllObjIndex[5];
	int index = 0;
	
	for (int i = 0,i2=0; i < MaxTracingDepth; i++)
	{
		Ray = normalize(Ray);

		float4 Get = rayGetPoint(Ray, Original);

			if (NO_POINT)
			{
				
				break;
			}
			else
			{

				index = i;
				int ObjIndex = Get.w;
				Color[i] = TriangelObj_Data[ObjIndex].emit;
				AllObjIndex[i] = ObjIndex;
				
				float3 Normal = TriangelObj_Data[ObjIndex].normal;
					Normal = normalize(Normal);

				if (TriangelObj_Data[ObjIndex].materialTypeX_SmoothY.x == MAT_Diffuse)
				{
					float r1 = 2 * 3.14159 * Rand_Data[offset.x + offset.y*1600].data[i2];
					i2 += 1;
					float r2 = Rand_Data[offset.x + offset.y * 1600].data[i2];
					i2 += 1;
					float r2s = sqrt(r2);
					float3 w = Normal;
						float3 u;
						if (abs(w.x) > 0.1)
						{
							u = GetCross(normalize(float3(0, 1, 0)), w);
						}
						else
						{
							u = GetCross(normalize(float3(1, 0, 0)), w);
						}
						u = normalize(u);
						float3 v = GetCross(w, u);
						Ray = normalize(GetMul(u, cos(r1)*r2s) + GetMul(v, sin(r1)*r2s) + GetMul(w,sqrt(1 - r2)));
				}
				else if (TriangelObj_Data[ObjIndex].materialTypeX_SmoothY.x == MAT_Reflect)
				{
					Ray = reflect(Ray, Normal);
				}	
					Original = Get.xyz;
			}

	}



	float4 returnColor = Color[index];
	
	for (int i2 = index -1; i2 >= 0; i2--)
	{
		returnColor = Color[i2] + TriangelObj_Data[AllObjIndex[i2]].reflect * returnColor;
	}

	return returnColor;
}







#define SUM_Sample_PerPixel 5000

[numthreads(16, 16, 1)]

void CS(int3 groupThreadID : SV_GroupThreadID,
	int3 dispatchThreadID : SV_DispatchThreadID)
{

	int x = dispatchThreadID.x;
	int y = dispatchThreadID.y;


	float posX = (x*widthPiexlSize - 1.333) / 2.145;
	float posY = (y*heightPiexlSize - 1.0) / 2.145;
	float3 pixelPos = float3(posX, -posY, 1);//2.145

		float3 ray = pixelPos - eyePos;


		float4 Get = rayGetPoint(ray, eyePos);

		gOutput[int2(x, y)] = gInput[int2(x, y)] + PathTracing(ray, eyePos, int2(x, y)) / (float)SUM_Sample_PerPixel;
	
	
}

