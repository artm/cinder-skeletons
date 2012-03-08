#include <cinder/app/AppBasic.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>
#include <cinder/Capture.h>
#include <cinder/Rand.h>
#include <cinder/params/Params.h>

#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include <boost/range/irange.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>

#include <vector>
#include <string>
#include <iostream>

using namespace ci;
using namespace ci::app;
using namespace ci::params;

#define MAIN_GUI_TITLE "Skeleton"
#define CONFIG_FILE ".skeleton"

class Skeleton : public AppBasic {
    virtual ~Skeleton() {
        InterfaceGl::save();
    }

    void setup() {
        Rand::randomize();

        InterfaceGl::load( std::string(getenv("HOME")) + "/" + CONFIG_FILE );
        m_gui = InterfaceGl( MAIN_GUI_TITLE, Vec2i( 250, 250 ) );

        std::vector< Capture::DeviceRef > devices = Capture::getDevices(true);
        if (devices.size()>0) {
            m_capture = Capture(320,240,devices[0]);
            m_capture.start();
        }

        m_texture = gl::Texture(320,240);
    }

    void update() {
        if (m_capture.isCapturing() && m_capture.checkNewFrame()) {
            // capture a frame
            Surface8u input = m_capture.getSurface();
            m_texture.update(input);
        }
    }

    void draw() {
        gl::clear( Color(0,0,0), true );

        gl::draw(m_texture, Rectf(0.0f,0.0f,(float)getWindowWidth(),(float)getWindowHeight()));
        params::InterfaceGl::draw();
    }

    void resize(ResizeEvent e) {
    }

    void toggleFullscreen() {
        setFullScreen(! isFullScreen() );
    }

    // fields
    gl::Texture m_texture;
    InterfaceGl m_gui;
    Capture m_capture;
};

CINDER_APP_BASIC( Skeleton, RendererGl )
