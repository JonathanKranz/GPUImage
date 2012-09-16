/**
 * Author: Alessio Placitelli
 * Contact: a.placitelli _@_ a2p.it
 *
 */

#include "GPUImage3x3ConvolutionFilter.h"
#include "GLProgram.h"

const std::string GPUImage3x3ConvolutionFilter::kGPUImage3x3ConvolutionFragmentShaderString(
"precision highp float;\
\
 uniform sampler2D inputImageTexture;\
 \
 uniform mediump mat3 convolutionMatrix;\
 \
 varying vec2 textureCoordinate;\
 varying vec2 leftTextureCoordinate;\
 varying vec2 rightTextureCoordinate;\
 \
 varying vec2 topTextureCoordinate;\
 varying vec2 topLeftTextureCoordinate;\
 varying vec2 topRightTextureCoordinate;\
 \
 varying vec2 bottomTextureCoordinate;\
 varying vec2 bottomLeftTextureCoordinate;\
 varying vec2 bottomRightTextureCoordinate;\
 \
 void main()\
 {\
     mediump vec4 bottomColor = texture2D(inputImageTexture, bottomTextureCoordinate);\
     mediump vec4 bottomLeftColor = texture2D(inputImageTexture, bottomLeftTextureCoordinate);\
     mediump vec4 bottomRightColor = texture2D(inputImageTexture, bottomRightTextureCoordinate);\
     mediump vec4 centerColor = texture2D(inputImageTexture, textureCoordinate);\
     mediump vec4 leftColor = texture2D(inputImageTexture, leftTextureCoordinate);\
     mediump vec4 rightColor = texture2D(inputImageTexture, rightTextureCoordinate);\
     mediump vec4 topColor = texture2D(inputImageTexture, topTextureCoordinate);\
     mediump vec4 topRightColor = texture2D(inputImageTexture, topRightTextureCoordinate);\
     mediump vec4 topLeftColor = texture2D(inputImageTexture, topLeftTextureCoordinate);\
\
     mediump vec4 resultColor = topLeftColor * convolutionMatrix[0][0] + topColor * convolutionMatrix[0][1] + topRightColor * convolutionMatrix[0][2];\
     resultColor += leftColor * convolutionMatrix[1][0] + centerColor * convolutionMatrix[1][1] + rightColor * convolutionMatrix[1][2];\
     resultColor += bottomLeftColor * convolutionMatrix[2][0] + bottomColor * convolutionMatrix[2][1] + bottomRightColor * convolutionMatrix[2][2];\
\
     gl_FragColor = resultColor;\
 }"
);

GPUImage3x3ConvolutionFilter::GPUImage3x3ConvolutionFilter() 
    : GPUImage3x3TextureSamplingFilter(), convolutionMatrixUniform_(0) {
    
    initWithFragmentShaderFromString(kGPUImage3x3ConvolutionFragmentShaderString);
    
    convolutionMatrixUniform_ = filterProgram_->getUniformIndex("convolutionMatrix");

    GPUMatrix3x3 init = {
        {0.f, 0.f, 0.f},
        {0.f, 1.f, 0.f},
        {0.f, 0.f, 0.f}
    };

    convolutionKernel_ = init;
}

GPUImage3x3ConvolutionFilter::~GPUImage3x3ConvolutionFilter() {

}

void GPUImage3x3ConvolutionFilter::setConvolutionKernel(const GPUMatrix3x3& kernel) {

    convolutionKernel_ = kernel;
    
    setMatrix3f(convolutionKernel_, convolutionMatrixUniform_, filterProgram_);
}