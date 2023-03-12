#ifndef TMP36_H
#define TMP36_H

class Tmp36 : public Pollable {
public:
    Tmp36();
    ~Tmp36();
    virtual void poll();
}

#endif
