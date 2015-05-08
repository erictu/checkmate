package com.example.erictu.chess_communication;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.util.Log;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.os.StrictMode;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.io.DataInputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.DataOutputStream;

public class MainActivity extends ActionBarActivity {

    Button sendButton;
    Button receiveButton;
    EditText mEdit;
    TextView mDisplay;
    EditText cEdit;
    private static final int REQUEST_ENABLE_BT = 1;
    private BluetoothAdapter btAdapter = null;
    private BluetoothSocket btSocket = null;
    private OutputStream outStream = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
//        receiveButton = (Button) findViewById(R.id.receive_button);
        sendButton = (Button) findViewById(R.id.send_button);
        mEdit = (EditText) findViewById(R.id.edit_message);
        cEdit = (EditText) findViewById(R.id.connection_edit);
//        mDisplay = (TextView) findViewById(R.id.move_text);
        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);
        sendButton.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        Log.v("EditText", mEdit.getText().toString());
                        Log.v("cEdit", cEdit.getText().toString());
//                        try {
//                            Socket s = new Socket("10.142.53.53", 8000);
//                            DataOutputStream dos = new DataOutputStream(s.getOutputStream());
//                            dos.writeUTF(mEdit.getText().toString());
//
//
////                            DataInputStream dis2 = new DataInputStream(s.getInputStream());
////                            InputStreamReader disR2 = new InputStreamReader(dis2);
////                            BufferedReader br = new BufferedReader(disR2);//create a BufferReader object for input
//                        } catch (IOException e) {
//                            e.printStackTrace();
//                        }

                        try{
                            Socket soc=new Socket(cEdit.getText().toString(),13000);
                            DataOutputStream dout=new DataOutputStream(soc.getOutputStream());
                            dout.writeUTF(mEdit.getText().toString());
                            dout.flush();
                            dout.close();
                            soc.close();
                        }catch(Exception e){
                            e.printStackTrace();
                        }

                    }
                }
        );
//        receiveButton.setOnClickListener(
//                new View.OnClickListener() {
//                    @Override
//                    public void onClick(View v) {
//
//                        String received = "Not What We Want";
//                        try {
//                            Socket soc2 = new Socket("10.142.53.53", 13000);
//                            DataInputStream dis2 = new DataInputStream(soc2.getInputStream());
//                            InputStreamReader disR2 = new InputStreamReader(dis2);
//                            BufferedReader br = new BufferedReader(disR2);//create a BufferReader object for input
//                            received = br.toString();
//                        } catch (IOException e) {
//                            e.printStackTrace();
//                        }
//
//                        mDisplay.setText(received);
//                        Log.v("what we got", received);
//                    }
//                }
//        );


    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
