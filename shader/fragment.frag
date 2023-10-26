#version 450 core
out vec4 FragColor;
in vec3 texcoord;
in vec3 objectSpaceCoord;  //首先我们要知道这个是世界空间坐标.因此采样的方向应该是摄像机的世
//界坐标到这个片元坐标的向量归一化


uniform vec3 cameraPos;
uniform sampler3D headTexture;
uniform sampler2D tex2D;

//这几个变量回头可能都会改成uniform
int stepNum = 500;
vec4 SliceMin = vec4(0,0,0,1);
vec4 SliceMax = vec4(1,1,1,1);
float Intensity = 1;
float Density = 0.001;
float Contrast = 1.0;
vec4 TintColor= vec4(0.6f,0.6f,1.0f,1.0f);
void main()
{

   float stepSize = 1.732f/stepNum;
   vec3 rayStartPos = objectSpaceCoord+vec3(0.5f);
   vec3 rayDir = normalize(objectSpaceCoord-cameraPos);
   FragColor = vec4(rayStartPos, 1.0f);
   
   float pixelValue = texture(headTexture, vec3(0.4,0.6,0.6)).r;
   float value = texture(headTexture, vec3(0.61)).r;
   //FragColor = vec4(vec3(value),1.0f);
   //FragColor = texture(headTexture, vec3(rayStartPos.xy,0.4));
   
   vec4 col = vec4(0.0f);
   float maxDensity = 0;
   float clipV = 0;
   for(uint istep = 0;istep<stepNum;++istep){
      float t = istep*stepSize;
      vec3 currPos = rayStartPos + rayDir*t;
      if (currPos.x < -0.001f || currPos.x>1.001f || currPos.y < -0.001f || currPos.y>1.001f || currPos.z < -0.001f || currPos.z>1.001f) {
			break;
		}
      if(currPos.x<SliceMin.x||currPos.x>SliceMax.x||currPos.y<SliceMin.y||currPos.y>SliceMax.y||currPos.z<SliceMin.z||currPos.z>SliceMax.z){
         continue;
      }

      float c = texture(headTexture, currPos).r*Intensity;
      vec4 src = vec4(c);
      src.a *=(1-Density)*pow(src.a, Contrast);
      src.rgb *= src.a;
      col = (1-col.a)*src+col;
      if(col.a>1) break;
   }
   FragColor = col*TintColor;

}