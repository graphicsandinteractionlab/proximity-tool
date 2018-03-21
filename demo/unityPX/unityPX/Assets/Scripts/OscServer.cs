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

			// smooth the values a bit
			depth = Mathf.Lerp (depth, float.Parse(msg.data[0].ToString()), 0.5f);

			// use only reasonable range
			if (depth > 300.0f && depth < 4500.0f && affectedObject) {

				Vector3 p = affectedObject.transform.localPosition;
				
				p.z = depth / 1000.0f; // convert to meter

				affectedObject.transform.localPosition = p;
			}
        }
    }
}