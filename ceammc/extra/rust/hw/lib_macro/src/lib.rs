use proc_macro::TokenStream;
use quote::quote;
use syn::{parse_macro_input, DeriveInput};

#[proc_macro_derive(PdError)]
pub fn impl_make_error_macro(input: TokenStream) -> TokenStream {
    let input = parse_macro_input!(input as DeriveInput);
    let name = input.ident;
    let gen = quote! {
        impl MakePdMessage<#name> for #name {
            fn pd_error(msg: CString) -> #name {
                #name::Message(hw_msg_level::Error, msg)
            }

            fn pd_debug(msg: CString) -> #name {
                #name::Message(hw_msg_level::Debug, msg)
            }

            fn pd_info(msg: CString) -> #name {
                #name::Message(hw_msg_level::Info, msg)
            }
        }
    };

    gen.into()
}
