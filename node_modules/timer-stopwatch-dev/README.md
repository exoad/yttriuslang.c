# timer-stopwatch

[![dependencies Status](https://david-dm.org/jackymancs4/timer-stopwatch/status.svg)](https://david-dm.org/jackymancs4/timer-stopwatch) [![devDependencies Status](https://david-dm.org/jackymancs4/timer-stopwatch/dev-status.svg)](https://david-dm.org/jackymancs4/timer-stopwatch?type=dev)

A stopwatch and countdown clock module for node.js, maintained.


## Install

```shell
npm install timer-stopwatch
```

## How to

**The Gist**

```javascript
new Stopwatch([countDownMS], [options]);
```

**Basic timers**

```javascript
var Stopwatch = require("timer-stopwatch");

var timer = new Stopwatch(60000); // A new countdown timer with 60 seconds
var stopwatch = new Stopwatch(); // A new count up stopwatch. Starts at 0.
```

### Event Methods

```javascript
// Fires every 50ms by default. Change setting the 'refreshRateMS' options
timer.onTime(function(time) {
	console.log(time.ms); // number of milliseconds past (or remaining);
	console.log(time.seconds); // number of seconds past (or remaining);
});

// Fires when the timer is done
timer.onDone(function() {
	console.log("Timer is complete");
});

// Fires when the timer is almost complete - default is 10 seconds remaining. Change with 'almostDoneMS' option
timer.onAlmostDone(function() {
	console.log("Timer is almost complete");
});

//These methods are chainable
timer
	.onTime(cb)
	.onAlmostDone(cb)
	.onDone(cb);
```

### Timer Methods

```javascript
timer.start();
timer.stop();
timer.startstop(); // Toggles the running state
timer.reset(countDownMS); // optional countDownMS to reset countdown to that many milliseconds
timer.getState() // Return 0, 1 or 2 if the timer is respectively stopped, running or complete
timer.isStopped()
timer.isRunning()
timer.isComplete()
```

### Properties

```javascript
timer.ms; // Number of milliseconds on the clock
timer.seconds; // Number of seconds on the clock
```

### Options

```javascript
var options = {
	refreshRateMS: 10, // How often the clock should be updated
	almostDoneMS: 10000 // When counting down - this event will fire with this many milliseconds remaining on the clock
};

var timer = new Stopwatch(60000, options);
```

## Testing

Unit and Integration tests (requires dev dependencies)

```shell
npm test
```

## Thanks to

@MickCrozier - The maintainer of the [original package.](https://github.com/MickCrozier/timer-stopwatch)

Other contributors:

* @danhere
* @connorjburton
* @RafaelKaczmarczyk
* @nitzo

## License

MIT License
