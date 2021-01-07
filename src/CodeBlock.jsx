import React from 'react'
import Highlight, { defaultProps } from 'prism-react-renderer';

function CodeBlock(props) {
    return (
        <Highlight {...defaultProps} code={props.code} language="cpp" theme={props.theme}>
            {({ className, style, tokens, getLineProps, getTokenProps }) => (
                <pre className={className} style={style}>
                    {tokens.map((line, i) => (
                        <div key={i} {...getLineProps({ line, key: i })}>
                            {line.map((token, key) => (
                                <span key={i} {...getTokenProps({ token, key })} />
                            ))}
                        </div>
                    ))}
                </pre>
            )}
        </Highlight>
    );
}

export default CodeBlock;