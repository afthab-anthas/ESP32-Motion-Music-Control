import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:permission_handler/permission_handler.dart';
import 'wifi_credentials_page.dart';

class BLEScanPage extends StatefulWidget {
  @override
  _BLEScanPageState createState() => _BLEScanPageState();
}

class _BLEScanPageState extends State<BLEScanPage> {
  List<ScanResult> scanResults = [];
  bool isScanning = false;

  @override
  void initState() {
    super.initState();
    requestPermissions();
  }

  Future<void> requestPermissions() async {
    await [
      Permission.bluetooth,
      Permission.bluetoothScan,
      Permission.bluetoothConnect,
      Permission.location
    ].request();

    startScan();
  }

  void startScan() {
    scanResults.clear();
    setState(() => isScanning = true);

    FlutterBluePlus.startScan(timeout: Duration(seconds: 5)).listen((result) {
      if (!scanResults.any((r) => r.device.id == result.device.id)) {
        setState(() => scanResults.add(result));
      }
    }, onDone: () {
      setState(() => isScanning = false);
    });
  }

  void connectToDevice(BluetoothDevice device) async {
    await device.connect(timeout: Duration(seconds: 10), autoConnect: false);
    Navigator.push(
      context,
      MaterialPageRoute(
        builder: (_) => WifiCredentialsPage(device: device),
      ),
    );
  }

  Widget buildScanButton() {
    return ElevatedButton.icon(
      icon: Icon(Icons.refresh),
      label: Text('Scan Again'),
      onPressed: isScanning ? null : startScan,
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text("Scan for ESP32")),
      body: Column(
        children: [
          if (isScanning) LinearProgressIndicator(),
          buildScanButton(),
          Expanded(
            child: ListView.builder(
              itemCount: scanResults.length,
              itemBuilder: (context, index) {
                final device = scanResults[index].device;
                final rssi = scanResults[index].rssi;
                return ListTile(
                  title: Text(device.name.isNotEmpty ? device.name : "Unknown"),
                  subtitle: Text("RSSI: $rssi"),
                  trailing: ElevatedButton(
                    child: Text("Connect"),
                    onPressed: () => connectToDevice(device),
                  ),
                );
              },
            ),
          ),
        ],
      ),
    );
  }
}
