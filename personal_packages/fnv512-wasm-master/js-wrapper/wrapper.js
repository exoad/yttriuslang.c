if(typeof createFnv512Module === 'undefined'){
    createFnv512Module = Promise.reject(new Error('fnv512 wasm module was not available'));
}

var fnv512 = {
    
    internal: {
        module: null,
        bytesFromBuffer: function(internalBuffer,bufLen){
            const resultView = new Uint8Array(this.module.HEAP8.buffer, internalBuffer, bufLen);//view, not a copy
            const result = new Uint8Array(resultView);//copy, not a view!
            return result;
        },

        bufferFromBytes: function(bytes){
            var internalBuffer = this.create_buffer(bytes.length);
            this.applyBytesToBuffer(bytes, internalBuffer);
            return internalBuffer;
        },
        applyBytesToBuffer : function(bytes, internalBuffer){
            this.module.HEAP8.set(bytes, internalBuffer);
        },
        toHex: function(bytes){
            return Array.prototype.map.call(bytes, function (n) {
                return (n < 16 ? '0' : '') + n.toString(16)
              }).join('');
        },
        inputToBytes: function (input) {
            if (input instanceof Uint8Array) {
                return input;
            } else if (typeof (input) === 'string') {
                return (new TextEncoder()).encode(input);
            } else {
                throw new Error('Input must be an string, Buffer or Uint8Array')
            }
        }
    },
    variants: {
       FNV_VARIANT_0: 0,
       FNV_VARIANT_1: 1,
       FNV_VARIANT_1A: 2
    },


    /**
     * Checks if Fnv512 support is ready (WASM Module loaded)
     * @return {Boolean}
     */
    isReady : function(){
        return this.internal.module!==null;
        
    },
    
    /**
     * Initializes a Hashing Context for Fnv512
     * @param {Number} digest_size the number of bits for the digest size (512 or 256). 512 is default.
     * @return {Object} the context object for this hashing session. should only be used to hash one data source.
     */
    init: function(digest_size,variant){
        if(typeof variant==="undefined") variant = this.variants.FNV_VARIANT_1A;
	digest_size=512;
        return {
            digest_size : digest_size,
	    variant: variant,
            context: this.internal.init(variant)
        };
    },
    
    /**
     * Update the hashing context with new input data
     * @param {Object} contextObject the context object for this hashing session
     * @param {Uint8Array} bytes an array of bytes to hash
     */
    update: function(contextObject, bytes){
        var inputBuffer = this.internal.bufferFromBytes(bytes);
        this.internal.update(contextObject.context, inputBuffer, bytes.length);
        this.internal.destroy_buffer(inputBuffer);
    },
    
    /**
     * Update the hashing context with new input data
     * @param {Object} contextObject the context object for this hashing session
     * @param {Object} value the value to use as bytes to update the hash calculation. Must be String or Uint8Array.
     */
    updateFromValue: function(contextObject, value){
        return this.update(contextObject, this.internal.inputToBytes(value));
    },
    
    /**
     * Finalizes the hashing session and produces digest ("hash") bytes.
     * Size of the returned array is always digest_size bytes long.
     * This method does not clean up the hashing context - be sure to call cleanup(ctx)!
     * @param {Object} contextObject the context object for this hashing session
     * @return {Uint8Array} an array of bytes representing the raw digest ("hash") value.
     */
    final: function(contextObject){
        var digestByteLen = contextObject.digest_size/8;
        var digestBuffer = this.internal.create_buffer(digestByteLen);
        //console.log("create buffer "+digestBuffer)
        //this.internal.final(contextObject.context,digestBuffer,digestByteLen);
        this.internal.final(contextObject.context,digestBuffer);
        
        var digestBytes = this.internal.bytesFromBuffer(digestBuffer, digestByteLen);
        //console.log("destroying buffer "+digestBuffer)
        this.internal.destroy_buffer(digestBuffer);
        return digestBytes;
    },
    
    /**
     * Cleans up and releases the Context object for the (now ended) hashing session.
     * @param {Object} contextObject the context object for this hashing session
     */
    cleanup: function(contextObject){
        this.internal.cleanup(contextObject.context);
    },
    
    /**
     * Calculates the Fnv512 message digest ("hash") for the input bytes or string
     * @param {Object} input the input value to hash - either Uint8Array or String
     * @param {Number} digest_size the number of bits for the digest size (512 or 256). 512 is default.
     * @return {Uint8Array} an array of bytes representing the raw digest ("hash") value.
     */
    digest: function (input, digest_size,variant) {
        input = this.internal.inputToBytes(input);
        var ctx = this.init(digest_size,variant);
        this.update(ctx,input);
        var bytes = this.final(ctx);
        this.cleanup(ctx);
        return bytes;
    },
    
    /**
     * Calculates the Fnv512 message digest ("hash") for the input bytes or string
     * @param {Object} input the input value to hash - either Uint8Array or String
     * @param {Number} digest_size the number of bits for the digest size (512 or 256). 512 is default.
     * @return {String} a hexadecimal representation of the digest ("hash") bytes.
     */
    digestHex: function (input, digest_size,variant) {
        var bytes = this.digest(input,digest_size,variant);
        return this.internal.toHex(bytes);
    }
};

createFnv512Module().then(async module => {
    fnv512.version= module.cwrap('version', 'number', []);
    fnv512.internal.create_buffer= module.cwrap('create_buffer', 'number', ['number']);
    fnv512.internal.destroy_buffer= module.cwrap('destroy_buffer', '', ['number']);
    fnv512.internal.init= module.cwrap('fnv512_init', '', ['number']);
    fnv512.internal.update= module.cwrap('fnv512_update', '', ['number','number','number']);
    fnv512.internal.final= module.cwrap('fnv512_final', '', ['number','number']);
    fnv512.internal.cleanup= module.cwrap('fnv512_cleanup', '', ['number']);
    fnv512.internal.module = module;
});
