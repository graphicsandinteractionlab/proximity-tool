/*
Copyright (c) 2011-2013 Gerhard Reitmayr, TU Graz

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef INTERFACE_RGBD_HPP
#define INTERFACE_RGBD_HPP

class RGBD {
protected:

    long _numFrames = 0;
    bool _wantsClose = false;


public:
    enum RGBDDevice {
        kRGBDDeviceKinect,
        kRGBDDeviceOpenNI2,
        kRGBDRealSense,
		kRGBDMSKinect1
    };

    RGBD() {}
    virtual ~RGBD() {}

    virtual int open() = 0;
    virtual bool available() const { return false; }
    virtual int update() = 0;
    virtual void close() = 0;

    /**
     * @brief create a RGBD capture device
     * @param device device type / backend
     * @param flags initial flags for the backend
     * @return instance of a RGBD capture device (null otherwise)
     */
    static RGBD* create(RGBDDevice device,const char* flags = "");

    long getNumFrames() const;


    virtual const void *getRGB(int& w,int& h) { return nullptr; }

    virtual const void *getDepth(int& w,int& h) { return nullptr; }

    bool getWantsClose() const;
    void setWantsClose(bool wantsClose);
};

#endif
