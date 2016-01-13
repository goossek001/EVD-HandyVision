//***************************************************************************************
// Open CameraFeed, take picture and store this in the shared lib.
// Autors:	Tim Rijssemus
// Date:	December 2015
// Version: 1.00
//***************************************************************************************

package kayentim.handyvision;

import android.hardware.Camera;
import android.os.Bundle;
import android.util.Log;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.TextView;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import java.util.Timer;
import java.util.TimerTask;


public class MainActivity extends AppCompatActivity {

    public native String getPicture(byte[] data);   //create the native functioncall
    private Camera mCamera = null;
    private CameraView mCameraView = null;

    static {
        System.loadLibrary("SharedLib"); // load the module name from Android.mk
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) { // creates the app, setting up camera ect.
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        try {
            mCamera = Camera.open();        // Opens back camera
        } catch (Exception e) {
            Log.d("ERROR", "Failed to get camera: " + e.getMessage());
        }

        if (mCamera != null) {
            mCameraView = new CameraView(this, mCamera);            //create a SurfaceView to show camera data
            FrameLayout camera_view = (FrameLayout) findViewById(R.id.camera_view);
            camera_view.addView(mCameraView);                       //add the SurfaceView to the layout
        }

        MyTimerTask myTask = new MyTimerTask();
        Timer myTimer = new Timer();
        myTimer.schedule(myTask, 3000, 1000);       // timer to take pictures (first will be made at the 3th sec, every following sec follows another picture)

        //btn to close the application
        ImageButton imgClose = (ImageButton) findViewById(R.id.imgClose);
        imgClose.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                System.exit(0);
            }
        });

    }

    class MyTimerTask extends TimerTask {
        public void run() {
            mCamera.takePicture(null, null, mPicture);
            Log.d("MyCameraApp", "I TOOK A PICTURE!!!!!!");
            mCamera.startPreview();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.d("MainAct:", "On Destroy .....");
    }

    /**
     * Picture Callback for sending the picture data to c file.
     */
    Camera.PictureCallback mPicture = new Camera.PictureCallback() {
        @Override
        public void onPictureTaken(byte[] data, Camera camera) {
            Log.d("MyCameraApp", "picture data send");
            ((TextView) findViewById(R.id.text_sign)).setText(getPicture(data));    // sends data to native file, and set the text on screen
        }
    };


}
