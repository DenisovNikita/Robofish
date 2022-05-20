#include <Servo.h>
#include <assert.h>
//#define SETTING

const int SMALL_DELAY   = 100;
const int DEFAULT_DELAY = 500;
const int BIG_DELAY     = 1000;

struct Robo_fish {
  enum {
    MOVE_FORWARD,
    MOVE_LEFT,
    MOVE_RIGHT,
    STOP,
  };

  static const int NUM_SERVOS = 3;
  const int CENTER[NUM_SERVOS] = {
    82,
    96,
    88,
  };
  const int SMALL_DELTA[NUM_SERVOS] = {
    5, 
    10, 
    15,
  };
  const int DEFAULT_DELTA[NUM_SERVOS] = {
    10, 
    20, 
    30,
  };
  const int BIG_DELTA[NUM_SERVOS] = {
    20, 
    30, 
    45,
  };
  const int MIN_BORDERS_FORWARD[NUM_SERVOS] = {
    CENTER[0] - DEFAULT_DELTA[0],
    CENTER[1] - DEFAULT_DELTA[1],
    CENTER[2] - DEFAULT_DELTA[2],
  };
  const int MAX_BORDERS_FORWARD[NUM_SERVOS] = {
    CENTER[0] + DEFAULT_DELTA[0],
    CENTER[1] + DEFAULT_DELTA[1],
    CENTER[2] + DEFAULT_DELTA[2],
  };
  const int MIN_BORDERS_LEFT[NUM_SERVOS] = {
    CENTER[0] - SMALL_DELTA[0],
    CENTER[1] - SMALL_DELTA[1],
    CENTER[2] - SMALL_DELTA[2],
  };
  const int MAX_BORDERS_LEFT[NUM_SERVOS] = {
    CENTER[0] + BIG_DELTA[0],
    CENTER[1] + BIG_DELTA[1],
    CENTER[2] + BIG_DELTA[2],
  };
  const int MIN_BORDERS_RIGHT[NUM_SERVOS] = {
    CENTER[0] - BIG_DELTA[0],
    CENTER[1] - BIG_DELTA[1],
    CENTER[2] - BIG_DELTA[2],
  };
  const int MAX_BORDERS_RIGHT[NUM_SERVOS] = {
    CENTER[0] + SMALL_DELTA[0],
    CENTER[1] + SMALL_DELTA[1],
    CENTER[2] + SMALL_DELTA[2],
  };
  
  Servo servo[NUM_SERVOS];
  int pos[NUM_SERVOS];

  int current_state = STOP;

  void modify_state(int input) {
    if (input == 'F') {
      current_state = MOVE_FORWARD;
    } else if (input == 'L') {
      current_state = MOVE_LEFT;
    } else if (input == 'R') {
      current_state = MOVE_RIGHT;
    } else if (input == 'S') {
      current_state = STOP;
    } else {
      assert(0);
    }
  }

  void rotate(Servo *servo, int *current_position, const int LEFT_BORDER, const int RIGHT_BORDER) {
    if (*current_position != LEFT_BORDER) {
      *current_position = LEFT_BORDER;
    } else  {
      *current_position = RIGHT_BORDER;
    }
    servo->write(*current_position);
  }
  
  void move_forward() {
    for (int i = 0; i < NUM_SERVOS; i++) {
      rotate(&servo[i], &pos[i], MIN_BORDERS_FORWARD[i], MAX_BORDERS_FORWARD[i]);
    }
  }

  void move_left() {
    for (int i = 0; i < NUM_SERVOS; i++) {
      rotate(&servo[i], &pos[i], MIN_BORDERS_LEFT[i], MAX_BORDERS_LEFT[i]);
    }
  }

  void move_right() {
    for (int i = 0; i < NUM_SERVOS; i++) {
      rotate(&servo[i], &pos[i], MIN_BORDERS_RIGHT[i], MAX_BORDERS_RIGHT[i]);
    }
  }

  void move_stop() {
    for (int i = 0; i < NUM_SERVOS; i++) {
      servo[i].write(CENTER[i]);
    }
  }
};

Robo_fish my_fish;
const char SIMPLE_GREETINGS[] = "F, L, R or S accepts";
const char GREETINGS[] = "Type 'F' for move forward, 'L' for move left, 'R' for move right ans 'S' for stop moving";

int is_correct_input(int input) {
  return input == 'F' || 
         input == 'L' ||
         input == 'R' || 
         input == 'S';
}

void setup() {
  Serial.begin(9600);
  my_fish.servo[0].attach(2);
  my_fish.servo[1].attach(3);
  my_fish.servo[2].attach(4);
  Serial.println(SIMPLE_GREETINGS);
  Serial.flush();
#ifdef SETTING
  pinMode(A0, INPUT);
#endif
}

void loop() {
  delay(SMALL_DELAY);
#ifdef SETTING
  int rotat = map(analogRead(A0), 0, 1023, 0, 180);
  Serial.println(rotat);
  Serial.flush();
  my_fish.servo[0].write(rotat); // 82 is center
  my_fish.servo[1].write(rotat); // 96 is center
  my_fish.servo[2].write(rotat); // 88 is center
  return;
#endif
  if (Serial.available()) {
    int input = Serial.read();
    if (input != 10) { // enter button
      Serial.print("You typed: ");
      Serial.println(input);
      Serial.flush();
      if (is_correct_input(input)) {
        my_fish.modify_state(input);
        Serial.print("Current state: ");
        Serial.println(my_fish.current_state);
        Serial.flush();
      }
    }
  } else {
    delay(DEFAULT_DELAY);
    if (my_fish.current_state == my_fish.MOVE_FORWARD) {
      Serial.println("Moving forward...");
      Serial.flush();
      my_fish.move_forward();
    } else if (my_fish.current_state == my_fish.MOVE_LEFT) {
      Serial.println("Moving left...");
      Serial.flush();
      my_fish.move_left();
    } else if (my_fish.current_state == my_fish.MOVE_RIGHT) {
      Serial.println("Moving right...");
      Serial.flush();
      my_fish.move_right();
    } else if (my_fish.current_state == my_fish.STOP) {
      Serial.println("Stopped...");
      Serial.flush();
      my_fish.move_stop();
    }else {
      assert(0);
    }
  }
}
