/*
 * Interface RGBD - (c) 2018 Hartmut Seichter
 *
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
