package sensor.project;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.Context;
import android.os.Bundle;

import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.app.Activity;
import android.content.Intent;
import android.os.CountDownTimer;
import android.widget.Button;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class MainActivity extends Activity {
    private static final int SERVERPORT = 80;

    // hardcoded IP, this should be set to the IP that the ESP32 sets gives itself, (((hopefully it stays the same)))
    public static String SERVER_IP = "192.168.221.89";

    private static boolean flag = true;

    private static boolean flag_ = true;

    private Accelerometer accelerometer;

    private Gyroscope gyroscope;

    public static Socket socket;
    public static Socket socket_2;

    BufferedReader inFromServer;

    public void onCreate(Bundle paramBundle) {
        super.onCreate(paramBundle);
        setContentView(R.layout.activity_main);

        

        if (flag) {

            // using this, the IP can be manually set
            /*
            AlertDialog.Builder builder = new AlertDialog.Builder((Context)this);
            builder.setTitle("IP");
            builder.setMessage("Please specify the IP to connect to");
            final EditText input_ = new EditText((Context)this);
            EditText editText = null;
            builder.setView((View)input_); // this either needs to be created here or this is an id in the mainactivity
            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface param1DialogInterface, int param1Int) {
                    MainActivity.SERVER_IP = input_.getText().toString();

                }
            });
            builder.show();
            */

            // without this some errors showed up, i left it this way
            (new CountDownTimer(1000L, 1000L) {
                public void onFinish() {

                    (new Thread(new ClientThread())).start();
                    // (new Thread(new ClientThread_NO2())).start();
                }

                public void onTick(long param1Long) {}
            }).start();
            flag = false;
        }


        /*
        * the basic mechanism is simply using some Listeners on each of the buttons,
        * and then using the Socket's sendUrgentData method, which is one byte
        *
        * then at the receiving arduino end, corresponding action takes place
        * according to the send UrgentData
        *
        * java yelled at me when not using try-catch  */

        Button forwardButton = (Button) findViewById(R.id.Forward);
        Button backwardButton = (Button) findViewById(R.id.Backward);

        Button leftButton = (Button) findViewById(R.id.Left);
        Button rightButton = (Button) findViewById(R.id.Right);

        Button disconnectButton = (Button) findViewById(R.id.Disconnect);
        Button sensormodeButton = (Button) findViewById(R.id.Sensormode);

        Button lightModeButton = (Button) findViewById(R.id.lightSet);

        TextView distanceSensorReadValue = (TextView) findViewById(R.id.distanceSensorReadValue);

        forwardButton.setOnTouchListener(new OnTouchListener() {
            @SuppressLint("ClickableViewAccessibility")
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if(event.getAction() == MotionEvent.ACTION_DOWN) {
                    try {
                        MainActivity.socket.sendUrgentData(70);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    try {
                        MainActivity.socket.sendUrgentData(87);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    try {
                        MainActivity.socket.sendUrgentData(88);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    try {
                        MainActivity.socket.sendUrgentData(90);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                /*
                try {
                    distanceSensorReadValue.setText(inFromServer.readLine());
                } catch (IOException e) {
                    AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
                    e.printStackTrace();

                    builder.setTitle("ERROR");
                    builder.setView((View) forwardButton);
                    builder.setMessage(e.getMessage());
                    builder.show();
                }

                 */
                return true;
            }
        });

        backwardButton.setOnTouchListener(new OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if(event.getAction() == MotionEvent.ACTION_DOWN) {
                    try {
                        MainActivity.socket.sendUrgentData(66);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    try {
                        MainActivity.socket.sendUrgentData(87);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    try {
                        MainActivity.socket.sendUrgentData(88);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    try {
                        MainActivity.socket.sendUrgentData(90);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                /*
                distanceSensorReadValue.setText("inFromServer.readLine()");
                char[] buf = new char[20];
                try {
                    distanceSensorReadValue.setText(inFromServer.read(buf));
                } catch (IOException e) {
                    e.printStackTrace();
                }


                 */
                return true;
            }
        });

        // 76, 84
        leftButton.setOnTouchListener(new OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if(event.getAction() == MotionEvent.ACTION_DOWN) {
                    try {
                        MainActivity.socket.sendUrgentData(76);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    try {
                        MainActivity.socket.sendUrgentData(84);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    try {
                        MainActivity.socket.sendUrgentData(77);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    try {
                        MainActivity.socket.sendUrgentData(68);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                /*
                try {
                    distanceSensorReadValue.setText(inFromServer.read());
                } catch (IOException e) {
                    e.printStackTrace();
                }
                distanceSensorReadValue.setText("inFromServer.readLine()");

                 */
                return true;
            }
        });

        // 82, 84
        rightButton.setOnTouchListener(new OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if(event.getAction() == MotionEvent.ACTION_DOWN) {
                    try {
                        MainActivity.socket.sendUrgentData(82);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    try {
                        MainActivity.socket.sendUrgentData(84);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    try {
                        MainActivity.socket.sendUrgentData(77);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    try {
                        MainActivity.socket.sendUrgentData(68);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
/*
                char[] barray = new char[20];
                try {
                    distanceSensorReadValue.setText(inFromServer.read(barray, 0, 20));
                } catch (IOException e) {
                    e.printStackTrace();
                }
                distanceSensorReadValue.setText("inFromServer.readLine()");

 */
                return true;


            }
        });

        lightModeButton.setOnTouchListener(new OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if(event.getAction() == MotionEvent.ACTION_DOWN) {
                    try {
                        MainActivity.socket.sendUrgentData(74);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    try {
                        MainActivity.socket.sendUrgentData(73);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
/*
                char[] barray = new char[20];
                try {
                    distanceSensorReadValue.setText(inFromServer.read(barray, 0, 20));
                } catch (IOException e) {
                    e.printStackTrace();
                }
                distanceSensorReadValue.setText("inFromServer.readLine()");

 */
                return true;


            }
        });

        disconnectButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                {
                    try {
                        MainActivity.socket.sendUrgentData(68);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    try {
                        MainActivity.socket.sendUrgentData(67);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    distanceSensorReadValue.setText("inFromServer.readLine()");
                }
            }
        });

        sensormodeButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                {
                    /* this creates a new intent which starts the SensorActivity
                    *  pauses this, closes the socket here
                    *  and the SensorActivity will start another socket*/
                    Intent intent = new Intent(MainActivity.this, SensorActivity.class);
                    try {
                        MainActivity.socket.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    onPause();
                    startActivity(intent);
                }
            }
        });

        // some color changing based on the tilting of the phone, unused
        /*
        this.accelerometer = new Accelerometer((Context)this);
        this.gyroscope = new Gyroscope((Context)this);
        final ConstraintLayout linearLayout = (ConstraintLayout)findViewById(R.id.linearLayout);
        linearLayout.setBackgroundColor(-3355444);
        ((ConstraintLayout)findViewById(R.id.linearLayout)).setBackgroundColor(110);
        this.gyroscope.setListener(new Gyroscope.Listener() {
            public void onRotation(float param1Float1, float param1Float2, float param1Float3) throws IOException {
                if (param1Float2 > 1.7F) {
                    linearLayout.setBackgroundColor(-16711681);
                    MainActivity.socket.sendUrgentData(103);
                    MainActivity.socket.sendUrgentData(103);
                    MainActivity.socket.sendUrgentData(103);
                } else if (param1Float2 < -1.7F) {
                    linearLayout.setBackgroundColor(-12303292);
                    MainActivity.socket.sendUrgentData(121);
                    MainActivity.socket.sendUrgentData(121);
                    MainActivity.socket.sendUrgentData(121);
                }
            }
        });
         */

        // could be done with "flag" i guess, this okay for now
        if (flag_) {
            flag_ = false;
        } else {
            (new Thread(new ClientThread())).start();
            // (new Thread(new ClientThread_NO2())).start();
        }


    }



    protected void onPause() {
        super.onPause();
        //this.accelerometer.unregister();
        //this.gyroscope.unregister();
    }

    protected void onResume() {
        super.onResume();
        //this.accelerometer.register();
        //this.gyroscope.register();
    }

    // the threaded client part
    /*
    * the inetAddress gets the global MainActivity.SERVER_IP
    * and using that ip, and port 80 it starts a simple socket server
    * */
    class ClientThread implements Runnable {
        public void run() {
            InetAddress inetAddress = null;
            try {
                InetAddress inetAddress1 = InetAddress.getByName(MainActivity.SERVER_IP);
                inetAddress = inetAddress1;
            } catch (UnknownHostException unknownHostException) {
                unknownHostException.printStackTrace();
            }
            try {
                MainActivity mainActivity = MainActivity.this;
                Socket socket = new Socket(inetAddress, 80);
                inFromServer = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                mainActivity.socket = socket;
            } catch (IOException iOException) {
                iOException.printStackTrace();
            }
        }
    }

    class ClientThread_NO2 implements Runnable {
        public void run() {
            try {
                MainActivity mainActivity = MainActivity.this;
                BufferedReader inFromServer = new BufferedReader(new InputStreamReader(mainActivity.socket.getInputStream()));
                mainActivity.inFromServer = inFromServer;
            } catch (IOException iOException) {
                iOException.printStackTrace();
            }
        }
    }

    public class ExceptionHandler implements
            Thread.UncaughtExceptionHandler {
        private final Activity myContext;
        private final String LINE_SEPARATOR = "\n";

        public ExceptionHandler(Activity context) {
            myContext = context;
        }

        public void uncaughtException(Thread thread, Throwable exception) {
            StringWriter stackTrace = new StringWriter();
            exception.printStackTrace(new PrintWriter(stackTrace));
            StringBuilder errorReport = new StringBuilder();
            errorReport.append("************ CAUSE OF ERROR ************\n\n");
            errorReport.append(stackTrace.toString());
            Intent intent = new Intent(myContext, MainActivity.class); //start a new activity to show error message
            intent.putExtra("error", errorReport.toString());
            myContext.startActivity(intent);

            android.os.Process.killProcess(android.os.Process.myPid());
            System.exit(10);
        }
    }

    /*
    class OutputThread implements Runnable {
        OutputThread(String message) {
            this.message = message;
        }
        @Override
        public void run() {
            output.write(message);
            output.flush();
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    tvMessages.append("client: " + message + "\n");
                    etMessage.setText("");
                }
            });
        }
    }
    */
    // new Thread(new ClientThread()).start();

    class ClientThread_YUN implements Runnable {
        @Override
        public void run() {
            Socket socket = null;
            try {
                InetAddress serverAddr = InetAddress.getByName("192.168.1.240");
                socket = new Socket(serverAddr, 8888);
                if(socket == null)System.out.println("SOCKET NULL");
                PrintWriter out = new PrintWriter(new BufferedWriter(
                        new OutputStreamWriter(socket.getOutputStream())),true);
                BufferedReader inFromServer = new BufferedReader(new InputStreamReader(socket.getInputStream()));

                while(true){
                    String msgFromServer = inFromServer.readLine();
                    System.out.println(msgFromServer);

                }
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            } finally {
                if (socket != null) {
                    System.out.println("STOP SOCKET");
                    // close socket
                }
            }
        }
    }

}
