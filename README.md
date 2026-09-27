# IoT-Based Smart Helmet for Accident Detection and Emergency Alert

## Overview

This project presents an IoT-based smart helmet designed to detect potential accident-related events and automatically notify a registered contact through email.

The system is built around an ESP32 microcontroller and integrates an MPU6050 motion sensor, vibration sensor, push button, and 16×2 I2C LCD display. The sensors continuously monitor the rider's motion and vibration conditions.

When a vibration event is detected, the system activates a 10-second alert period. During this time, the rider can press the push button to cancel the alert in case of a false alarm. If the alert is not cancelled within 10 seconds, the ESP32 sends an emergency email notification using Wi-Fi connectivity.

The LCD provides real-time information about the vibration status and the motion status obtained from the MPU6050.

## Problem Statement

Two-wheeler riders are particularly vulnerable during road accidents because they do not have the protective enclosure available in cars.

After an accident, a rider may be unconscious or unable to communicate with others. This can delay the process of notifying family members or emergency contacts.

This project aims to address this issue by developing a smart helmet that can detect potential accident-related vibration events and automatically send an email alert when the rider does not cancel the alert.

## Key Features

- Real-time vibration monitoring
- MPU6050-based motion monitoring
- Stable and unstable motion classification
- Automatic accident-alert sequence
- 10-second alert cancellation period
- Push-button false-alarm cancellation
- ESP32 Wi-Fi connectivity
- Automated email notification
- 16×2 LCD status display
- Compact and portable design

## System Architecture

The system consists of the following major components:

```text
                 MPU6050
              Motion Sensor
                   │
                   │
                   ▼
Vibration Sensor ─► ESP32 ◄── Push Button
                   │
                   │
             ┌─────┴─────┐
             │           │
             ▼           ▼
            LCD        Wi-Fi
                         │
                         ▼
                 Email Notification
