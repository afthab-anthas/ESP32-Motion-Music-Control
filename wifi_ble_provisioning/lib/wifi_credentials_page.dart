import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';

class WifiCredentialsPage extends StatelessWidget {
  final BluetoothDevice device;

  WifiCredentialsPage({required this.device});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text("Enter Wi-Fi Credentials")),
      body: Center(
        child: Text("Connected to ${device.name}"),
      ),
    );
  }
}
