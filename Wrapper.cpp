
#define WIN32_LEAN_AND_MEAN		

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include "cameralibrary.h"
using namespace CameraLibrary;
using namespace System;
using namespace System::Runtime::InteropServices;

namespace NPCameraSDKDotNet{

	public enum class VideoMode
    {
        SegmentMode              = 0,
        GrayscaleMode            = 1,
        ObjectMode               = 2,
        InterleavedGrayscaleMode = 3,
        PrecisionMode            = 4,
        BitPackedPrecisionMode   = 5,
        MJPEGMode                = 6,
        VideoModeCount              ,
        UnknownMode              
    };

	public enum class StatusLEDs
    {
        GreenStatusLED      = 0x20,
        RedStatusLED        = 0x10,
        CaseStatusLED       = 0x40,
        IlluminationLED     = 0x80,

        LeftRedStatusLED    = 16,
        LeftGreenStatusLED  = 32,
        RightRedStatusLED   = 1,
        RightGreenStatusLED = 2,
    };


	public ref class MObject{
	public:
		MObject(cObject * cobject){
			this->cobject = cobject;
		}
		~MObject(){
			delete this->cobject;
		}

		int Left(){return cobject->Left();}
		int Right(){return cobject->Right();}
		int Top(){return cobject->Top();}
		int Bottom(){return cobject->Bottom();}
		float Area(){return cobject->Area();}

		float Radius(){return cobject->Radius();}
		float Aspect(){return cobject->Aspect();}
		float Roundness(){return cobject->Roundness();}

		int Width(){return cobject->Width();}
		int Height(){return cobject->Height();}

		float X(){return cobject->X();}
		float Y(){return cobject->Y();}

        ////== Creation & processing functions ==--
        //
        //void Clear();
        //void AddSegment    ( Segment* segment );
        //void AddSegmentList( cObject * objectSource );

        //void PurgeReferences();

        ////== Generate object information from attached segments ==--
        //void ProcessLineSegments        ();
        //void ProcessWeightedLineSegments();
        //void ProcessSubPixelLineSegments();
        //void CalculateDimensions        ();

        //Segment *     SegmentHead();
        //Segment *     SegmentTail();

		MObject ^ Clone(){
			cObject * object = new cObject(*this->cobject);
			return gcnew MObject(object);
		}
	protected:
	private:
		cObject * cobject;
	};


	public ref class MFrame{
	public:
		MFrame(Frame * frame){
			this->frame = frame;
		}
		~MFrame(){
			delete this->frame;
		}

		int ObjectCount(){return frame->ObjectCount();}          
		int FrameID(){return frame->FrameID();}             
        //eVideoMode      FrameType();            //== Frame Type =============================----
        //int             MJPEGQuality();         //== For MJPEG Frames, MJPEG Quality (1-100) ----

		MObject ^ Object(int index){
			cObject * obj = frame->Object(index);
			if (obj == 0){
				return nullptr;
			} else {
				return gcnew MObject(obj);
			}
		}
        //ObjectLink *    GetLink(int index);
        //Camera *        GetCamera();            //== Reference to originating camera ========----

		bool IsInvalid(){return frame->IsInvalid();}          
		bool IsEmpty(){return frame->IsEmpty();}              
		bool IsGrayscale(){return frame->IsGrayscale();}      

		double TimeStamp(){return frame->TimeStamp();}        

		void Release(){frame->Release();}             

		int RefCount(){return frame->RefCount();}
		void AddRef(){frame->AddRef();}

        ////== Rasterization Functionality ==========--

        //void            Rasterize(unsigned int Width, unsigned int Height, unsigned int Span,
        //                          unsigned int BitsPerPixel, void *Buffer);

        //void            Rasterize(Bitmap *BitmapRef);

        ////== Frame Decompression Functionality (Advanced Feature) ======---

        //void            PopulateFrom(CompressedFrame *Frame);

        ////== Uncommonly Needed Methods =================================---

        //unsigned char*  GetGrayscaleData();
        //int             GetGrayscaleDataSize();
		void CopyGrayScaleDataTo(IntPtr buffer) {
			const int width = frame->GetCamera()->Width();
			const int height = frame->GetCamera()->Height();
			frame->Rasterize(width, height, width, 8, buffer.ToPointer());
		}

	protected:
	private:
		Frame * frame;

	};

	typedef void (__stdcall *BANG_MESSAGE)(void);
	delegate void TakesABangDelegate();

	class CameraListener : public cCameraListener {
	public:
		CameraListener(){
			this->FrameAvailableCallback = 0;
			this->FrameOverflowCallback = 0;
			this->ButtonPressedCallback = 0;
		}

		void SetFrameAvailableCallback(BANG_MESSAGE function){
			this->FrameAvailableCallback = function;
		}

		void SetFrameOverflowCallback(BANG_MESSAGE function){
			this->FrameOverflowCallback = function;
		}
		
		void SetButtonPressedCallback(BANG_MESSAGE function){
			this->ButtonPressedCallback = function;
		}

		void FrameAvailable(){
			if (this->FrameAvailableCallback != 0)
				this->FrameAvailableCallback();
		}

        void FrameOverflow(){
			if (this->FrameOverflowCallback != 0)
				this->FrameOverflowCallback();
		}

		void ButtonPressed(){
			if (this->ButtonPressedCallback != 0)
				this->ButtonPressedCallback();
		}

	protected:
		BANG_MESSAGE FrameAvailableCallback;
		BANG_MESSAGE FrameOverflowCallback;
		BANG_MESSAGE ButtonPressedCallback;
	};

	public ref class MCamera{
        //Camera();
        //~Camera();

        //virtual void PacketTest(int PacketCount) {};


        ////== Intended Public Command Interface ==============================================----

		void        SetGrayscaleDecimation(int Value){camera->SetGrayscaleDecimation(Value);}


        //void        SetLateMJPEGDecompression(bool Enable);
        //bool        LateMJPEGDecompression();

        //int         Serial();
        //int         Model();
        //int         Revision();
        //int         HardwareInterface();
        //void        ResetFrameCount();

        //virtual bool CameraIDValid();

        //virtual void SetIRFilter(bool Enabled);    //== Enable/Disable IR Bandpass Filter ===----
        //virtual bool IRFilter();
        //virtual bool IsFilterSwitchAvailable()     { return false;     }


        //virtual void SetHighPowerMode(bool Enable) {};
        //virtual bool HighPowerMode()               { return false;     }
        //virtual bool IsHighPowerModeAvailable()    { return false;     }
        //virtual bool IsHighPowerModeSupported()    { return false;     }
        //int          LowPowerSetting();

        //void         SetMJPEGQuality(int Value);   //== Set MJPEG Quality Level (0-100) =====----
        //virtual int  MJPEGQuality();
        //virtual bool IsMJPEGAvailable()            { return false; }

        //int          SwitchState();                //== SmartNav4's switch state ============----

        //HealthMonitor * Health();                  //== Camera Health Information ===========----

        //virtual void GetDistortionModel(Core::DistortionModel &Model) {}; //== Distortion Model -

        ////== Status LEDs ====================================================================----
        //
        

        ////== Camera Physical Constants ======================================================----

        //virtual double FocalLength()         { return 0.0; }

        ////== Additional =====================================================================----

        //void        SetTextOverlay  (bool Enable);
        //void        SetMarkerOverlay(bool Enable);

        //bool        TextOverlay();
        //bool        MarkerOverlay();
        //
        //void        SetName(const char *Name);

        //
        //InputManager::ConnectionTypes ConnectionType();  //== Returns type of input USB, etc. ---

        ////== Less Commonly Used Public Methods ==============================================----

        //virtual void AttachInput(cInputBase * Input);    //== Manually attach a camera input =---
 
        //bool IsCommandQueueEmpty();                      //== Know if camera is busy with ====---
        //                                                 //== communicating with the device ==---

        //void        ReleaseFrame(Frame* frame);          //== Alternative release frame call =---

        //const char* DevicePath();                        //== Returns Device Path for USB ====---

        //virtual void SendCommand(cCameraCommand * Command); //== Push a camera command =======---
        //void AttachModule  (cCameraModule  * Module);    //== Attach additional functionality ---
        //void RemoveModule  (cCameraModule  * Module);    //== Remove additional functionality ---
        //void AttachListener(cCameraListener * Listener); //== Attach for camera events =======---
        //void RemoveListener(cCameraListener * Listener); //== Remove camera listener =========---
        //void Shutdown();                                 //== Permanently shutdown camera ====---

        //virtual bool IsHardwareKey () { return false; }  //== For separation of cameras & keys --
        //virtual bool IsHub ()         { return false; }  //== For separation of cameras & Hubs --
        //virtual bool IsUSB ()         { return true;  }  //== Camera helpers =================---
        //virtual bool IsEthernet()     { return false; }  //== Camera helpers =================---
        //virtual bool IsTBar ()        { return false; }  //== Camera helpers =================---

	public:

		MCamera(Camera * camera){
			this->camera = camera;
			this->cameraListener = new CameraListener();
			this->camera->AttachListener(this->cameraListener);

			TakesABangDelegate^ fp = gcnew TakesABangDelegate(this, & MCamera::NewFrame);
			this->callbackHandle = GCHandle::Alloc(fp);
			IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
			BANG_MESSAGE cb = static_cast<BANG_MESSAGE>(ip.ToPointer());
			this->cameraListener->SetFrameAvailableCallback(cb);
		}

		~MCamera(){
			callbackHandle.Free();
			delete this->cameraListener;
			delete this->camera;
		}

		event EventHandler<EventArgs^> ^ FrameAvailable;
		
		void NewFrame(){
			this->FrameAvailable(this, gcnew EventArgs());
		}

		MFrame ^ GetFrame(){
			Frame * frame;
			frame = camera->GetFrame();
			if (frame == 0){
				return nullptr;
			} else {
				return gcnew MFrame(frame);
			}
		}

		MFrame ^ GetLatestFrame(){
			Frame * frame;
			frame = camera->GetFrame();
			if (frame == 0){
				return nullptr;
			} else {
				return gcnew MFrame(frame);
			}
		}         

        //const char* Name();                           //== Returns name of camera ===========----
        //

		void SetLED(StatusLEDs LED, bool Enable){
			eStatusLEDs leds = static_cast<eStatusLEDs>(LED);
			camera->SetLED(leds, Enable);
		}
        //void SetAllLED(eStatusLEDs LED);                //== Turn Camera LEDs On/Off ========----
        //void SetStatusIntensity(int Intensity);         //== All Status LED to (0-->255) ====----


		void        Start(){camera->Start();}  
		void        Stop(bool TurnNumericOff){camera->Stop(TurnNumericOff);} 
        //
		bool		IsValid(){return camera != 0;}
		bool        IsCameraRunning(){return camera->IsCameraRunning();}     

		void Release(){camera->Release();}		

		void        SetExposure(int Value){camera->SetExposure(Value);}  
		void        SetThreshold(int Value){camera->SetThreshold(Value);}        
		void        SetIntensity(int Value){camera->SetIntensity(Value);}        
		void        SetPrecisionCap(int Value){camera->SetPrecisionCap(Value);}  
		void        SetShutterDelay(int Value){camera->SetShutterDelay(Value);}  

		void        SetFrameRate(int Value){camera->SetFrameRate(Value);}        
        //void        SetFrameDecimation(int Value);        //== Set Camera Frame Decimation ==----

		int FrameRate(){return camera->FrameRate();}                         
        //int         FrameDecimation();                    //== Get Camera Frame Decimation ==----
        //int         GrayscaleDecimation();                //== Get Camera Grayscale Decimation --

		int         PrecisionCap(){return camera->PrecisionCap();}           
		int         ShutterDelay(){return camera->ShutterDelay();}           

		int         Exposure(){return camera->Exposure();}                   
		int         Threshold(){return camera->Threshold();}                 
		int         Intensity(){return camera->Intensity();}                 

		void SetVideoType(VideoMode Value){
			eVideoMode mode = static_cast<eVideoMode>(Value);
			camera->SetVideoType(mode);
		}

		VideoMode VideoType(){
			VideoMode mode;
			mode = static_cast<VideoMode>(camera->VideoType());
			return mode;
		}


		float       DataRate(){return camera->DataRate();}                   
		float       PacketSize(){return camera->PacketSize();}               

		void        ResetWindow(){camera->ResetWindow();}                    
		void        SetWindow(int X1, int Y1, int X2, int Y2){camera->SetWindow(X1,Y1,X2,Y2);} 

		void        SendEmptyFrames(bool Enable){camera->SendEmptyFrames(Enable);}
		void        SendInvalidFrames(bool Enable){camera->SendInvalidFrames(Enable);}


		void         SetAGC(bool Enable){camera->SetAGC(Enable);}       
		bool         AGC(){return camera->AGC();}
        bool IsAGCAvailable()              { return camera->IsAGCAvailable();}

		void         SetAEC(bool Enable){camera->SetAEC(Enable);}       
		bool         AEC(){return camera->AEC();}
        bool IsAECAvailable(){return camera->IsAECAvailable();}

        int    HardwareFrameRate()   { return camera->HardwareFrameRate();   }

		bool IsInitilized  (){return camera->IsInitilized();}
		bool IsDisconnected(){return camera->IsDisconnected();}

        //eCameraState State ();
		int UID(){return camera->UID();}
		int Model(){return camera->Model();}
		int Serial(){return camera->Serial();}


		void SetEnableBlockingMask(bool Enabled){camera->SetEnableBlockingMask(Enabled);}
		bool IsBlockingMaskEnabled(){return camera->IsBlockingMaskEnabled();}
		void AddBlockingRectangle   (int X1, int Y1, int X2, int Y2){camera->AddBlockingRectangle(X1,Y1,X2,Y2);}
        //void         RemoveBlockingRectangle(int X1, int Y1, int X2, int Y2);
		void SetBitMaskPixel(int X, int Y, bool Mask){camera->SetBitMaskPixel(X,Y,Mask);}
		void         ClearBlockingMask      () {camera->ClearBlockingMask();}
        //void         GetBlockingMask        (unsigned char *Buffer, int BufferSize);
        //void         SetBlockingMask        (const unsigned char *Buffer, int BufferSize);
		void UpdateBlockingMask     (){camera->UpdateBlockingMask();}
		int          BlockingMaskWidth      (){return camera->BlockingMaskWidth();}
		int          BlockingMaskHeight     (){return camera->BlockingMaskHeight();}
		int          BlockingGrid           (){return camera->BlockingGrid();}


		int PhysicalPixelWidth(){return camera->PhysicalPixelWidth();}
		int PhysicalPixelHeight(){return camera->PhysicalPixelHeight();}
		double ImagerWidth(){ return camera->ImagerWidth(); }
        double ImagerHeight(){ return camera->ImagerHeight(); }
		int Width(){return camera->Width();}
		int Height(){return camera->Height();}

		void SetObjectColor(int Color){
			camera->SetObjectColor(Color);
		}

		int ObjectColor(){
			return camera->ObjectColor();
		}

		bool IsCameraTempValid(){
			return camera->IsCameraTempValid();
		}

		float CameraTemp(){
			return camera->CameraTemp();
		}

		bool IsCameraFanSpeedValid(){
			return camera->IsCameraFanSpeedValid();
		}

		float CameraFanSpeed(){
			return camera->CameraFanSpeed();
		}

		int MinimumExposureValue(){
			return camera->MinimumExposureValue();
		}

		int MaximumExposureValue(){
			return camera->MaximumExposureValue();
		}

		int MinimumFrameRateValue(){
			return camera->MinimumFrameRateValue();
		}

		int MaximumFrameRateValue(){
			return camera->MaximumFrameRateValue();
		}

		int CameraID (){
			return this->camera->CameraID();
		}

		void SetNumeric(bool enable, int num){
			this->camera->SetNumeric(enable, num);
		}

	protected:
	private:
		Camera * camera;
		CameraListener * cameraListener;
		GCHandle callbackHandle;
	};

	public ref class MCameraManager{
	public:
		static void EnableDevelopment(){
			CameraLibrary_EnableDevelopment();
		}

		static bool WaitForInitialization(){
			return CameraManager::X().WaitForInitialization();
		}

		static bool AreCamerasInitialized(){
			return CameraManager::X().AreCamerasInitialized();
		}

		static void Shutdown(){
			CameraManager::X().Shutdown();
		}

		static MCamera^ GetCamera(){
			Camera * cam = CameraManager::X().GetCamera();
			if (cam == 0){
				return nullptr;
			} else {
				return gcnew MCamera(cam);
			}
		}

		static array<MCamera^>^ GetCameras(){
			CameraList list;
			array<MCamera^>^ ret = gcnew array<MCamera^>(list.Count());
			for (int c=0;c < list.Count(); c++){
				ret[c] = gcnew MCamera(CameraManager::X().GetCamera(list[c].UID()));
			}
			return ret;
		}
	};

	public enum class CameraState
    {
        Uninitialized = 0,
        InitializingDevice,
        InitializingCamera,
        Initializing,
        Initialized,
        Disconnected,
        Shutdown
    };

    public enum class SyncMode
    {
        SyncModeDefault = 0,
        SyncModeCustom,
        SyncModeShutterGoggles
    };

    public enum class SyncType
    {
        SyncTypeOptiSync = 0,
        SyncTypeWiredSync
    };

    public enum class SyncCameraSync
    {
        SyncAutoUSBSync = 0,
        SyncForceUSBSync
    };

    public enum class SyncInputSource
    {
        SyncInputSourceWired = 0,                 
        SyncInputSourceInternal = 0,
        SyncInputSourceSMPTEInput,
        SyncInputSourceSyncExternal=2,
        SyncInputSourceSyncExternalEitherEdge=2,
        SyncInputSourceSyncExternalRisingEdge,
        SyncInputSourceSyncExternalFallingEdge,
        SyncInputSourceSyncExternalHighGated,
        SyncInputSourceSyncExternalLowGated,
        SyncInputSourceUSB                        
    };

    public enum class SyncOutputPhase
    {
        SyncOutputInPhase = 0,
        SyncOutputDelayedByGlobalOffset
    };
    public enum class SyncOutputPulseDuration
    {
        SyncOutputPulseDurationExposureTime = 0,
        SyncOutputPulseDurationPassThrough,
        SyncOutputPulseDurationRecordingLevel,
        SyncOutputPulseDurationRecordingPulse
    };

    public enum class SyncOutputPolarity
    {
        SyncOutputPolarityNormal = 0,
        SyncOutputPolarityInverted
    };
    public enum class USBSyncInControl
    {
        USBSyncInControlGateClosedDisabled=0,
        USBSyncInControlGateOpenEnabled
    };




}
