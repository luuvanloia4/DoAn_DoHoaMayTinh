#ifndef IWidget_h
#define IWidget_h

class IGUIWidget {
public:
    enum class EAlign {
        NONE,
        BEGIN,
        CENTER,
        END
    };

    IGUIWidget() = default;
    virtual ~IGUIWidget() = default;
    IGUIWidget(const IGUIWidget&) = delete;
    IGUIWidget& operator=(const IGUIWidget&) = delete;
    IGUIWidget(IGUIWidget&&) = delete;
    IGUIWidget& operator=(IGUIWidget&&) = delete;

    virtual void Draw() const = 0;

protected:
    template <class T>
    void Align(T* value, const T& min, const T& max, EAlign align) const {
        if (align == EAlign::NONE) {
            return;
        }

        if (align == EAlign::BEGIN) {
            *value = min;
            return;
        } else if (align == EAlign::CENTER) {
            *value = (min + max) / 2;
            return;
        } else /*align == EAling::END*/ {
            *value = max;
            return;
        }
    }
};

#endif