class Toggle {
  int state;
  fun int value() { return state; }
  fun Toggle activate() {
    !state => state;
    return this;
  }
}

class NthToggle extends Toggle {
  int count, countMax;
  fun Toggle activate() {
    if(++count >= countMax) {
      (this $ Toggle).activate();
      0 => count;
    }
    return this;
  }
}

100000 => int n;

Toggle toggle;
true => int val => toggle.state;

repeat(n) {
  toggle.activate().value() => val;
  toggle.activate().value() => val;
  toggle.activate().value() => val;
  toggle.activate().value() => val;
  toggle.activate().value() => val;
  toggle.activate().value() => val;
  toggle.activate().value() => val;
  toggle.activate().value() => val;
  toggle.activate().value() => val;
  toggle.activate().value() => val;
}

<<< toggle.value() >>>;

NthToggle ntoggle;
true => val => ntoggle.state;
3 => ntoggle.countMax;

repeat(n) {
  ntoggle.activate().value() => val;
  ntoggle.activate().value() => val;
  ntoggle.activate().value() => val;
  ntoggle.activate().value() => val;
  ntoggle.activate().value() => val;
  ntoggle.activate().value() => val;
  ntoggle.activate().value() => val;
  ntoggle.activate().value() => val;
  ntoggle.activate().value() => val;
  ntoggle.activate().value() => val;
}

<<< ntoggle.value() >>>;
