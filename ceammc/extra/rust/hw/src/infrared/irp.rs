use irp::Irp;

pub enum Protocol {
    NEC,
    NEC_48,
    NEC_48_1,
    NEC_48_2,
}

pub fn get_irp(proto: Protocol) -> Irp {
    let irp = irp::Irp::parse(
        match proto {
            Protocol::NEC_48_1 => r#"{38.4k,564}<1,-1|1,-3>(16,-8,D:8,S:8,F:8,~F:8,E:8,~E:8,1,^108m,(16,-4,1,^108m)*)[D:0..255,S:0..255=255-D,F:0..255,E:0..255]"#,
            Protocol::NEC_48   => r#"{38.4k,564}<1,-1|1,-3>(16,-8,D:8,S:8,F:8,~F:8,E:8,~E:8,1,^108m)[D:0..255,S:0..255=255-D,F:0..255,E:0..255]"#,
            Protocol::NEC_48_2 => r#"{38.4k,564}<1,-1|1,-3>(16,-8,D:8,S:8,F:8,~F:8,E:8,~E:8,1,^108m)*[D:0..255,S:0..255=255-D,F:0..255,E:0..255]"#,
            Protocol::NEC      => r#"{38.4k,564}<1,-1|1,-3>(16,-8,D:8,S:8,F:8,~F:8,1,^108m) [D:0..255,S:0..255=255-D,F:0..255]"#,
        }
        
    )
    .expect("parse should succeed");

    irp
}
