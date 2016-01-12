//***************************************************************************************
// Open CameraFeed, take picture and store this in the shared lib.
// Autors:	Tim Rijssemus
// Date:	December 2015
// Version: 1.00
//***************************************************************************************

package kayentim.handyvision;

import android.hardware.Camera;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.util.Log;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.Menu;
import android.widget.TextView;
import android.widget.FrameLayout;
import android.widget.ImageButton;


public class MainActivity extends AppCompatActivity {

    public native String helloWorld();   //create the native functioncall
    private Camera mCamera = null;
    private CameraView mCameraView = null;

    static {
        System.loadLibrary("SharedLib"); // load the module name from Android.mk
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
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

        // makes a snapshot
        new CountDownTimer(3000,1500){

            @Override
            public void onFinish() {
                mCamera.takePicture(null, null, mPicture);
                Log.d("MyCameraApp", "I TOOK A PICTURE!!!!!!");
                mCamera.startPreview();
            }

            @Override
            public void onTick(long millisUntilFinished) {

            }
        }.start();

                //TODO Place the text in the screen
        ((TextView) findViewById(R.id.text_sign)).setText(helloWorld());  // this must set the text in the image.

        //btn to close the application
        ImageButton imgClose = (ImageButton) findViewById(R.id.imgClose);
        imgClose.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                System.exit(0);
            }
        });

    }

    /**
     * Picture Callback for sending the picture data to c file.
     */
    Camera.PictureCallback mPicture = new Camera.PictureCallback() {
        @Override
        public void onPictureTaken(byte[] data, Camera camera) {
            //TODO send this data to cpp file.
        }
    };

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

}
