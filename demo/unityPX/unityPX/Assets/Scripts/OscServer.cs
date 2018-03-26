using UnityEngine;
using System;
using System.Net;
using System.Net.Sockets;

public class OscServer : MonoBehaviour
{
    public int listenPort = 4455;				// osc server input

	public float depth = 0;

	public GameObject affectedObject;


    UdpClient udpClient;
    IPEndPoint endPoint;


	/// <summary>
	/// internal osc parser
	/// </summary>
    Osc.Parser osc = new Osc.Parser ();

    void Start ()
    {
        endPoint = new IPEndPoint (IPAddress.Any, listenPort);
        udpClient = new UdpClient (endPoint);

    }

    void Update ()
    {
        while (udpClient.Available > 0) {
            osc.FeedData (udpClient.Receive (ref endPoint));
        }

        while (osc.MessageCount > 0) {
			
            var msg = osc.PopMessage ();

//			Debug.Log (msg.data.Length);


			//
			// new version uses the following pattern:
			// 

			//			__________________
			//
			//			|  0	1	2
			//
			//			|  3	4	5
			//
			//			|  6	7	8




			float depthVal = float.Parse (msg.data [0].ToString ());

			Debug.Log (msg.path + " " + depthVal);

			// thresholding 
			if (depthVal < 100 || depthVal > 4300)
				continue;


			// smooth the values a bit
			depth = Mathf.Lerp (depth, depthVal, 0.5f);

			// use only reasonable range
			if (depth > 300.0f && depth < 4500.0f && affectedObject) {

				Vector3 p = affectedObject.transform.localPosition;
				
				p.z = depth / 1000.0f; // convert to meter

				affectedObject.transform.localPosition = p;
			}
        }
    }
}