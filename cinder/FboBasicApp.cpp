#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/Camera.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/ip/Fill.h"
#include "cinder/ip/Resize.h"
#include "cinder/Camera.h"
#include "cinder/Surface.h"
#include "cinder/ImageIo.h"


using namespace ci;
using namespace ci::app;
using namespace std;

// This sample shows a very basic use case for FBOs - it renders a spinning colored cube
// into an FBO, and uses that as a Texture onto the sides of a blue cube.
class FboBasicApp : public App {
  public:
	void	setup() override;
	void	update() override;
	void	draw() override;

  private:
	void			renderSceneToFbo();
	
	gl::FboRef			mFbo;
	mat4				mRotation;
	static const int	FBO_WIDTH = 2000, FBO_HEIGHT = 2000;
	Surface32f* mSurface;
	Surface* mSurface2;
	gl::TextureRef mAnisoLevelTexture;
	float* p;
};

void FboBasicApp::setup()
{
	gl::Fbo::Format format;
	//format.setSamples( 4 ); // uncomment this to enable 4x antialiasing
	mFbo = gl::Fbo::create( FBO_WIDTH, FBO_HEIGHT, format.depthTexture() );

	p = (float*)malloc(FBO_WIDTH * FBO_WIDTH * sizeof(float));
	memset(p, 0xFF, FBO_WIDTH * FBO_WIDTH * sizeof(float));

	mSurface = new Surface32f(FBO_WIDTH, FBO_WIDTH, true);
	memset(mSurface->getData(), 0xFF, FBO_WIDTH * FBO_WIDTH * sizeof(float));
	mSurface2 = new Surface(FBO_WIDTH, FBO_WIDTH, true);


	mAnisoLevelTexture = gl::Texture::create(*mSurface);


	gl::enableDepthRead();
	gl::enableDepthWrite();
}

// Render the color cube into the FBO
void FboBasicApp::renderSceneToFbo()
{
	// this will restore the old framebuffer binding when we leave this function
	// on non-OpenGL ES platforms, you can just call mFbo->unbindFramebuffer() at the end of the function
	// but this will restore the "screen" FBO on OpenGL ES, and does the right thing on both platforms
	gl::ScopedFramebuffer fbScp( mFbo );
	// clear out the FBO with blue
	gl::clear( Color( 0.25, 0.5f, 1.0f ) );

	// setup the viewport to match the dimensions of the FBO
	gl::ScopedViewport scpVp( ivec2( 0 ), mFbo->getSize() );

	// setup our camera to render the torus scene
	CameraPersp cam( mFbo->getWidth(), mFbo->getHeight(), 60.0f );
	cam.setPerspective( 60, mFbo->getAspectRatio(), 1, 1000 );
	cam.lookAt( vec3( 2.8f, 1.8f, -2.8f ), vec3( 0 ));
	gl::setMatrices( cam );

	// set the modelview matrix to reflect our current rotation
	gl::setModelMatrix( mRotation );
	
	// render the color cube
	gl::ScopedGlslProg shaderScp( gl::getStockShader( gl::ShaderDef().color() ) );
	gl::color( Color( 1.0f, 0.5f, 0.25f ) );
	gl::drawColorCube( vec3( 0 ), vec3( 2.2f ) );
	gl::color( Color::white() );
}

void FboBasicApp::update()
{
	// Rotate the torus by .06 radians around an arbitrary axis
	mRotation *= rotate( 0.06f, normalize( vec3( 0.16666f, 0.333333f, 0.666666f ) ) );
	
	// render into our FBO
	renderSceneToFbo();
}
bool sss = false;

void FboBasicApp::draw()
{
	// clear the window to gray
	gl::clear( Color( 0.35f, 0.35f, 0.35f ) );

	// setup our camera to render the cube
	CameraPersp cam( getWindowWidth(), getWindowHeight(), 60.0f );
	cam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
	cam.lookAt( vec3( 2.6f, 1.6f, -2.6f ), vec3( 0 ) );
	gl::setMatrices( cam );


	// draw a cube textured with the FBO
	{
		gl::ScopedGlslProg shaderScp( gl::getStockShader( gl::ShaderDef().texture() ) );
		gl::drawCube( vec3( 0 ), vec3( 2.2f ) );
	}

	// use the scene we rendered into the FBO as a texture
	mFbo->bindTexture();

	// show the FBO color texture in the upper left corner
	gl::setMatricesWindow( toPixels( getWindowSize() ) );
	gl::draw( mFbo->getColorTexture(), Rectf( 0, 0, 128, 128 ) );
	// and draw the depth texture adjacent
	gl::draw( mFbo->getDepthTexture(), Rectf( 128, 0, 256, 128 ) );

	mFbo->unbindTexture();


	//mSurface->set

	ip::fill(mSurface, ColorA(0.4f, 0.4f, 1.0f, 1));
	//uint32_t* p = mSurface->getData();
	//mSurface->getData();
	//for (int y = 0; y < mSurface->getHeight()/2; y++) {
	//	for (int x = 0; x < mSurface->getWidth()/2; x++) {
	//		//mSurface->setPixel(glm::ivec2(x, y), ColorA(1.0f, 0.4f, 0.4f, 1));

	//	}
	//}
	if (!sss) {
	Surface32f ss = ip::resizeCopy(*mSurface, Area(0, 0, mSurface->getWidth(), mSurface->getHeight()), glm::ivec2(256, 256));
	//mAnisoLevelTexture->update(*mSurface);
	mAnisoLevelTexture->update(*mSurface);
	sss = true;
	}
	gl::draw(mAnisoLevelTexture, Rectf(128+128, 0, 256+128, 128));


	//gl::Fbo::Format hdrFormat;
	//hdrFormat.setColorTextureFormat(gl::Texture::Format().internalFormat(GL_RGBA32F));
	//gl::FboRef myHdrFbo = gl::Fbo::create(640, 480, hdrFormat);
	//gl::draw(myHdrFbo->getColorTexture());





	/*ip::fill(mSurface2, ColorA(0.4f, 0.4f, 1.0f, 1));
	for (int y = mSurface2->getHeight()/2; y < mSurface2->getHeight(); y++) {
		for (int x = mSurface2->getWidth()/2; x < mSurface2->getWidth(); x++) {
			mSurface2->setPixel(glm::ivec2(x, y), ColorA(0.4f, 1.0f, 0.4f, 1));
		}
	}*/
	//gl::TextureRef mAnisoLevelTexture2 = gl::Texture::create(*mSurface2);
	//gl::draw(mAnisoLevelTexture2, Rectf(128 + 128 + 128, 0, 256 + 128 + 128, 128));
}

CINDER_APP( FboBasicApp, RendererGl )
