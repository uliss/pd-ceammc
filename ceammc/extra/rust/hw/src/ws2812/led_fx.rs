use palette::{FromColor, Hsv, Srgb};
use rgb::RGB8;

pub fn rainbow(idx: usize, count: usize, angle: f32) -> RGB8 {
    if idx > count || count < 1 {
        return RGB8 { r: 0, g: 0, b: 0 };
    }

    let hue = (((idx as f32 / count as f32) + angle).abs() % 1.0) * 360.0;

    let hsl = Hsv::new(hue, 1.0, 1.0);
    let rgb: Srgb<u8> = Srgb::from_color(hsl).into_format();

    RGB8 {
        r: rgb.red,
        g: rgb.green,
        b: rgb.blue,
    }
}

#[cfg(test)]
mod tests {
    use rgb::RGB8;

    use crate::ws2812::led_fx::rainbow;

    #[test]
    fn fx_rainbow() {
        assert_eq!(rainbow(0, 16, 0.0), RGB8::new(255, 0, 0));
        assert_eq!(rainbow(0, 16, 0.5), RGB8::new(0, 255, 255));
    }
}
