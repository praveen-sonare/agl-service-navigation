# Navigation Service

## Overview

Navigation service keeps track of application data that allows other clients to share location, waypoints,
and state between each other.

## Verbs

| Name                | Description                                 | JSON Response                      |
|---------------------|---------------------------------------------|------------------------------------|
| subscribe           | subscribe to an navigation service          | *Request:* {"value": "status"}     |
| unsubscribe         | unsubscribe to an navigation service        | *Request:* {"value": "status"}     |
| broadcast_status    | broadcast status event to other clients     | *Request:* {"state": "stop"}       |
| broadcast_position  | broadcast position event to other clients   | See *position Verb* section        |
| broadcast_waypoints | broadcast waypoints events to other clients | See *waypoints Verb* section       |


### broadcast_position Verb

Populate same data referenced in *position Event* section for the event

### broadcast_waypoints Verb

Populate same data referenced in *waypoints Event* section for the event

## Events

| Name           | Description                         |
|----------------|-------------------------------------|
| status         | status of the navigation engine     |
| position       | current route or car position event |
| waypoints      | waypoints for routing engine        |

### position Event

<pre>
{
  "points": [
    {
      "position": "car",
      "latitude": 36.12906,
      "longitude": -115.17908
      "direction": 273.0,
      "distance": 1530.45
    }
  ]
}
</pre>

### waypoints Event

<pre>
{
  "points": [
    {
      "latitude": 36.12906,
      "longitude": -115.17908
    }
  ]
}
</pre>
