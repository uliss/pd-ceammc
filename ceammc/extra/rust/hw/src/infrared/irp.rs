use std::{collections::HashMap, io::BufReader};

use irp::{Decoder, Irp, DFA};
use lazy_static::lazy_static;
use log::debug;
use std::str::FromStr;
use xml::reader::{EventReader, XmlEvent};

enum Element {
    None,
    Irp,
    AbsoluteTolerance,
    RelativeTolerance,
    AlternateName,
    DecodeOnly,
    Decodable,
    PreferOver,
    MinimumLeadout,
    RejectRepeatLess,
}

#[derive(Clone, Debug)]
struct ProtoParams {
    irp: String,
    abs_tolerance: u32,
    rel_tolerance: f32,
    min_leadout: u32,
}

fn create_proto_map() -> HashMap<String, ProtoParams> {
    let data = parse_xml().unwrap();

    let mut res = HashMap::new();

    for x in &data {
        res.insert(
            x.name.clone(),
            ProtoParams {
                irp: x.irp.clone(),
                abs_tolerance: x.absolute_tolerance,
                rel_tolerance: x.relative_tolerance,
                min_leadout: x.minimum_leadout,
            },
        );
    }

    res
}

fn parse_xml() -> Result<Vec<irp::Protocol>, String> {
    let data = include_bytes!("IrpProtocols.xml");

    let file = BufReader::new(data.as_slice());
    let parser = EventReader::new(file);

    let mut protocols: Vec<irp::Protocol> = Vec::new();
    let mut protocol = None;
    let mut element = Element::None;

    for e in parser {
        match e {
            Ok(XmlEvent::StartElement {
                name, attributes, ..
            }) => match name.local_name.as_ref() {
                "protocol" => {
                    if attributes.len() == 1 && attributes[0].name.local_name == "name" {
                        protocol = Some(irp::Protocol {
                            name: attributes[0].value.to_owned(),
                            decodable: true,
                            absolute_tolerance: 100,
                            relative_tolerance: 0.3,
                            minimum_leadout: 20000,
                            ..Default::default()
                        });
                    } else {
                        return Err(format!("missing name attribute"));
                    }
                }
                "irp" => {
                    element = Element::Irp;
                }
                "parameter" => {
                    for attr in attributes {
                        match attr.name.local_name.as_ref() {
                            "prefer_over" => {
                                element = Element::PreferOver;
                            }
                            "absolute-tolerance" => {
                                element = Element::AbsoluteTolerance;
                            }
                            "relative-tolerance" => {
                                element = Element::RelativeTolerance;
                            }
                            "decodable" => {
                                element = Element::Decodable;
                            }
                            "decode-only" => {
                                element = Element::DecodeOnly;
                            }
                            "alt_name" => {
                                element = Element::AlternateName;
                            }
                            "minimum-leadout" => {
                                element = Element::MinimumLeadout;
                            }
                            "reject_repeatless" => {
                                element = Element::RejectRepeatLess;
                            }
                            _ => (),
                        }
                    }
                }
                _ => (),
            },
            Ok(XmlEvent::CData(data)) => {
                if let Some(protocol) = &mut protocol {
                    match element {
                        Element::Irp => {
                            protocol.irp = data;
                        }
                        Element::AlternateName => {
                            protocol.alt_name.push(data);
                        }
                        Element::PreferOver => {
                            protocol.prefer_over.push(data);
                        }
                        Element::Decodable => {
                            protocol.decodable = bool::from_str(&data).unwrap();
                        }
                        Element::DecodeOnly => {
                            protocol.decode_only = bool::from_str(&data).unwrap();
                        }
                        Element::RejectRepeatLess => {
                            protocol.reject_repeatess = bool::from_str(&data).unwrap();
                        }
                        Element::AbsoluteTolerance => {
                            protocol.absolute_tolerance = u32::from_str(&data).unwrap();
                        }
                        Element::RelativeTolerance => {
                            protocol.relative_tolerance = f32::from_str(&data).unwrap();
                        }
                        Element::MinimumLeadout => {
                            protocol.minimum_leadout = u32::from_str(&data).unwrap();
                        }
                        Element::None => (),
                    }
                }

                element = Element::None;
            }
            Ok(XmlEvent::EndElement { name }) => {
                if name.local_name == "protocol" {
                    if let Some(protocol) = protocol {
                        protocols.push(protocol);
                    }
                    protocol = None;
                }
            }
            Err(e) => return Err(format!("Error: {e}")),
            _ => {}
        }
    }

    Ok(protocols)
}

fn get_irp(proto: &str) -> Result<(Irp, ProtoParams), String> {
    lazy_static! {
        static ref XML_PROTOCOLS: HashMap<String, ProtoParams> = create_proto_map();
    }

    match XML_PROTOCOLS.get(proto) {
        Some(x) => {
            let irp = irp::Irp::parse(&x.irp).unwrap();
            Ok((irp, x.clone()))
        }
        None => {
            let mut keys = XML_PROTOCOLS.keys().map(|x| x.clone()).collect::<Vec<_>>();
            keys.sort();
            return Err(format!(
                "unknown protocol: {proto}, supported values are: {}",
                keys.join(", ")
            ));
        }
    }
}

pub fn get_decoder<'a>(proto: String) -> Result<(DFA, Decoder<'a>), String> {
    let (irp, params) = get_irp(proto.as_str())?;

    let options = irp::Options {
        aeps: params.abs_tolerance,
        eps: (params.rel_tolerance * 100.0).round().clamp(0.0, 100.0) as u32,
        max_gap: 20000,
        ..Default::default()
    };

    debug!("proto params: {options:?}");

    let dfa = irp.compile(&options)?;
    Ok((dfa, irp::Decoder::new(options)))
}
