class OpticalComponent
{
protected:
    // Common attributes like position, size, etc.
    Position _position;
    Size _size;

public:
    // Constructor
    OpticalComponent(const Position &position, const Size &size) : _position(position), _size(size) {}

    // Getters
    Position getPosition() const { return _position; }
    Size getSize() const { return _size; }

    // Setters
    void setPosition(const Position &position) { _position = position; }
    void setSize(const Size &size) { _size = size; }

    // Virtual function for handling light interaction, to be overridden by derived classes
    virtual void handleLight(Photon *photon) = 0;
};

class RectangularComponent : public OpticalComponent
{
public:
    // Constructor
    RectangularComponent(const Position &position, const Size &size) : OpticalComponent(position, size) {}

    // Overridden function
    void handleLight(Photon *photon) override
    {
        // Handle light interaction specific to rectangular components
    }
};

// And similar for other types of components...
